#include <fstream>
#include <iostream>
#include <string>

#include "protoconf/hub.pc.h"
#include "protoconf/item_conf.pc.h"
#include "protoconf/registry.pc.h"
#include "protoconf/test_conf.pc.h"

void WriteFile(const std::string& filename, const std::string& input) {
  std::ofstream out(filename);
  out << input;
  out.close();
}

template <class T, const bool threaded = false>
class Singleton {
 private:
  Singleton(const T&) = delete;
  Singleton(T&&) = delete;
  void operator=(const T&) = delete;
  static inline T* GetInstancePtr() {
    T* ptr = nullptr;
    if (threaded) {
      static thread_local T* new_ptr = new T();
      ptr = new_ptr;
    } else {
      static T* new_ptr = new T();
      ptr = new_ptr;
    }
    assert(ptr != nullptr);
    return ptr;
  }

 public:
  static inline T& Instance() { return *GetInstancePtr(); }

 protected:
  Singleton() = default;
};

class MyHub : public Singleton<tableau::Hub, true> {};

// syntactic sugar
template <typename T>
const std::shared_ptr<T> Get() {
  return MyHub::Instance().Get<T>();
}

int main() {
  tableau::Registry::Init();
  bool ok = MyHub::Instance().Load("../../testdata/", [](const std::string& name) { return true; });
  if (!ok) {
    std::cout << "protobuf hub load failed: " << tableau::GetErrMsg() << std::endl;
    return 1;
  }
  auto item1 = MyHub::Instance().Get<tableau::ItemConf>();
  if (!item1) {
    std::cout << "protobuf hub get Item failed!" << std::endl;
    return 1;
  }
  std::cout << "item1: " << item1->Data().DebugString() << std::endl;

  //   auto activity_conf = MyHub::Instance().Get<tableau::ActivityConf>();
  //   if (!activity_conf) {
  //     std::cout << "protobuf hub get ActivityConf failed!" << std::endl;
  //     return 1;
  //   }

  //   const auto* section_conf = activity_conf->Get(100001, 1, 2);
  //   if (!section_conf) {
  //     std::cout << "ActivityConf get section failed!" << std::endl;
  //     return 1;
  //   }

  const auto* section_conf = MyHub::Instance().Get<tableau::ActivityConf, protoconf::Section>(100001, 1, 2);
  if (!section_conf) {
    std::cout << "ActivityConf get section failed!" << std::endl;
    return 1;
  }

  std::cout << "-----section_conf" << std::endl;
  std::cout << section_conf->DebugString() << std::endl;
  return 0;
}