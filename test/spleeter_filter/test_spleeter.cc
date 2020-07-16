#include <gtest/gtest.h>

#include "wave/file.h"

#include "spleeter_common/spleeter_common.h"
#include "spleeter_filter/filter.h"

TEST(Spleeter, Filter) {
  std::error_code err;

  // read a file
  wave::File file;
  std::string test_file(TEST_FILE);
//  std::string test_file("/Users/gvne/Desktop/snipet-HP.wav");
  file.Open(test_file, wave::kIn);
  std::vector<float> data;
  file.Read(&data);
  ASSERT_FALSE(err);

  // Initialize the filter
  const auto separation_type = spleeter::FourStems;
  spleeter::Initialize(std::string(SPLEETER_MODELS), {separation_type}, err);
  ASSERT_FALSE(err);
  spleeter::Filter filter(separation_type);
  filter.set_extra_frame_latency(10);
  filter.Init(err);
  ASSERT_FALSE(err);

  filter.set_OverlapLength(2);
  filter.set_FrameLength(filter.ProcessLength() - filter.OverlapLength());

  // Initialize the audio buffer
  const auto block_size = 2048;
  filter.set_block_size(block_size);
  rtff::Waveform buffer(block_size, filter.channel_count());
  std::vector<rtff::Waveform> outputs;
  for (auto source_idx = 0; source_idx < 4; source_idx++) {
    outputs.push_back(buffer);
  }

  // Run each frames
  auto multichannel_buffer_size = block_size * buffer.channel_count();
  for (uint32_t sample_idx = 0;
     sample_idx < data.size() - multichannel_buffer_size;
     sample_idx += multichannel_buffer_size) {

    float* sample_ptr = data.data() + sample_idx;
    buffer.fromInterleaved(sample_ptr);
    filter.Write(&buffer);
    filter.Read(outputs.data());
    // TODO: mix instead of just copying the first source
    outputs[0].toInterleaved(sample_ptr);
  }

  wave::File result_file;
  result_file.Open(std::string(OUTPUT_DIR) + "/result.wav", wave::kOut);
  result_file.set_sample_rate(44100);
  result_file.set_channel_number(2);
  result_file.Write(data);
}
