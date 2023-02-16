#include <benchmark/benchmark.h>

#include "common/signal_generator/signal_generator.h"

std::shared_ptr<AudioFile<float>> audioFile = nullptr;

std::shared_ptr<hpaslt::SignalGenerator> signalGenerator = nullptr;

static void signalGeneratorSetup(const benchmark::State& state) {
  audioFile = std::make_shared<AudioFile<float>>();
  audioFile->setNumChannels(2);

  signalGenerator = std::make_shared<hpaslt::SignalGenerator>();
  signalGenerator->bindAudioFile(audioFile);
}

static void signalGeneratorTeardown(const benchmark::State& state) {
  audioFile = nullptr;
  signalGenerator = nullptr;
}

static void changeSignalLengthBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    signalGenerator->changeLength(state.range(0));
  }
}

BENCHMARK(changeSignalLengthBenchmark)
    ->RangeMultiplier(2)
    ->Range(44100, 44100 << 4)
    ->Setup(signalGeneratorSetup)
    ->Teardown(signalGeneratorTeardown);

static void generateSignalBenchmark(benchmark::State& state) {
  signalGenerator->changeLength(state.range(0));
  for (auto _ : state) {
    signalGenerator->generateSignal(32);
  }
}

BENCHMARK(generateSignalBenchmark)
    ->RangeMultiplier(2)
    ->Range(44100, 44100 << 4)
    ->Setup(signalGeneratorSetup)
    ->Teardown(signalGeneratorTeardown);

BENCHMARK_MAIN();