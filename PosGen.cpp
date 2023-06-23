#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <vector>

#include "zmq.hpp"
#include "protobuf/SensorPosition.pb.h"
#include "Generator.h"

using DataList_t = std::vector<std::string>;
using namespace PositionGenerator;

std::string generateMessageData(const PositionGenerator::SensorPosition& Sensor, PositionGenerator::ChronoBasedGenerator& Gen)
{
  Vector3 PosWithNoise = Gen.addNoise(Sensor.position());
  GeneratedPosition Pos;
  Data3d* pCoord = Pos.mutable_position();
  pCoord->set_x(PosWithNoise.x());
  pCoord->set_y(PosWithNoise.y());
  pCoord->set_z(PosWithNoise.z());
  Pos.set_sensorid(Sensor.sensorId());
  Pos.set_timestamp_usec(Sensor.timestamp());

  std::string bytes;
  Pos.SerializeToString(&bytes);

  return bytes;
}

DataList_t generateMessagesForSingleLoop(PositionGenerator::ChronoBasedGenerator& Gen)
{
  DataList_t msgDataList;
  Gen.generateData();
  for (const auto& Sensor : Gen)
  {
    msgDataList.emplace_back(std::move(generateMessageData(Sensor, Gen)));
  }
  return msgDataList;
}

void messageLoop(std::atomic_bool& StopSignal, zmq::socket_t& socket, PositionGenerator::ChronoBasedGenerator& Gen, float FrequencyInHz)
{
  while (!StopSignal)
  {
    auto msgDataList = generateMessagesForSingleLoop(Gen);
    for (const auto& msgData : msgDataList)
    {
      zmq::const_buffer data(msgData.data(), msgData.size());
      auto res = socket.send(data, zmq::send_flags::none);
      if (!res.has_value() || res.value() == 0)
      {
        // error - for now just log to std::output
        std::cout << " transmission error \n";
      }
    }
    // now wait 1000ms
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.f/FrequencyInHz)));
  }
}

int main()
{
  constexpr const char BindAddress[] = "tcp://*:4646";
  std::cout << "This is PositionGenerator v0.1 \n";
  std::cout << "Generating Positions and publishing at " << BindAddress << "\n";

  using namespace PositionGenerator;

  // create Generator with suitable parameter
  float maxVelocity = 12.0; // 12m/s
  uint64_t timeStampUnitsPerSecond = 1000000; // microsec
  timestamp_t initialTime = 0;
  Vector3 minValues(0.f, 0.f, 0.2f);
  Vector3 maxValues(100.f, 100.f, 1.5f);
  int numSensors = 10;
  float noiseDimension = 0.3f;
  float FrequencyInHz = 10.f;

  ChronoBasedGenerator Gen(GenerationParameter()
    .setMaximalVelocity(maxVelocity)
    .setNumOfSensors(numSensors)
    .setInitialTimestamp(initialTime)
    .setBoundingCuboid(minValues, maxValues)
    .setTimestampUnitPerSecond(timeStampUnitsPerSecond)
    .setNoiseDimension(noiseDimension)
  );

  // scope to limit life time of async future and zmq sockets
  {
    zmq::context_t context;
    zmq::socket_t socket(context, zmq::socket_type::pub);
    socket.bind(BindAddress);
    std::atomic_bool StopSignal = false;
    auto voidFuture = std::async(messageLoop, std::ref(StopSignal), std::ref(socket), std::ref(Gen),FrequencyInHz);
    std::cout << "  >>> press RETURN to stop <<<\n ";
    getchar();
    StopSignal = true; // signal thread to quit
  }
  // at this point all zmq objects had their destructor called
  std::cout << "  stopped. \n";
}

