// Code generated by protoc-gen-cpp-tableau-loader. DO NOT EDIT.
// versions:
// - protoc-gen-cpp-tableau-loader v0.4.5
// - protoc                        v3.19.3
// source: test_conf.proto

#pragma once
#include <string>

#include "hub.pc.h"
#include "protoconf/test_conf.pb.h"

namespace tableau {
class ActivityConf : public Messager {
 public:
  static const std::string& Name() { return kProtoName; };
  virtual bool Load(const std::string& dir, Format fmt, const LoadOptions* options = nullptr) override;
  const protoconf::ActivityConf& Data() const { return data_; };

 private:
  virtual bool ProcessAfterLoad() override final;

 public:
  const protoconf::ActivityConf::Activity* Get(uint64_t key1) const;
  const protoconf::ActivityConf::Activity::Chapter* Get(uint64_t key1, uint32_t key2) const;
  const protoconf::Section* Get(uint64_t key1, uint32_t key2, uint32_t key3) const;
  const int32_t* Get(uint64_t key1, uint32_t key2, uint32_t key3, uint32_t key4) const;

 private:
  static const std::string kProtoName;
  protoconf::ActivityConf data_;

  // OrderedMap accessers.
 public:
  using int32_OrderedMap = std::map<uint32_t, int32_t>;
  const int32_OrderedMap* GetOrderedMap(uint64_t key1, uint32_t key2, uint32_t key3) const;

  using int32_Map = ::google::protobuf::Map<uint32_t, int32_t>;
  using protoconf_Section_OrderedMapValue = std::pair<int32_OrderedMap, const int32_Map*>;
  using protoconf_Section_OrderedMap = std::map<uint32_t, protoconf_Section_OrderedMapValue>;
  const protoconf_Section_OrderedMap* GetOrderedMap(uint64_t key1, uint32_t key2) const;

  using protoconf_Section_Map = ::google::protobuf::Map<uint32_t, protoconf::Section>;
  using Activity_Chapter_OrderedMapValue = std::pair<protoconf_Section_OrderedMap, const protoconf_Section_Map*>;
  using Activity_Chapter_OrderedMap = std::map<uint32_t, Activity_Chapter_OrderedMapValue>;
  const Activity_Chapter_OrderedMap* GetOrderedMap(uint64_t key1) const;

  using Activity_Chapter_Map = ::google::protobuf::Map<uint32_t, protoconf::ActivityConf::Activity::Chapter>;
  using Activity_OrderedMapValue = std::pair<Activity_Chapter_OrderedMap, const Activity_Chapter_Map*>;
  using Activity_OrderedMap = std::map<uint64_t, Activity_OrderedMapValue>;
  const Activity_OrderedMap* GetOrderedMap() const;

 private:
  Activity_OrderedMap ordered_map_;

  // Index accessers.
 public:
  using Index_AwardVector = std::vector<const protoconf::Item*>;
  using Index_AwardMap = std::unordered_map<uint32_t, Index_AwardVector>;
  const Index_AwardMap& FindAward() const;
  const Index_AwardVector* FindAward(uint32_t id) const;
  const protoconf::Item* FindFirstAward(uint32_t id) const;

 private:
  Index_AwardMap index_award_map_;

 public:
  using Index_ChapterVector = std::vector<const protoconf::ActivityConf::Activity::Chapter*>;
  using Index_ChapterMap = std::unordered_map<uint32_t, Index_ChapterVector>;
  const Index_ChapterMap& FindChapter() const;
  const Index_ChapterVector* FindChapter(uint32_t chapter_id) const;
  const protoconf::ActivityConf::Activity::Chapter* FindFirstChapter(uint32_t chapter_id) const;

 private:
  Index_ChapterMap index_chapter_map_;

 public:
  using Index_NamedChapterVector = std::vector<const protoconf::ActivityConf::Activity::Chapter*>;
  using Index_NamedChapterMap = std::unordered_map<std::string, Index_NamedChapterVector>;
  const Index_NamedChapterMap& FindNamedChapter() const;
  const Index_NamedChapterVector* FindNamedChapter(std::string chapter_name) const;
  const protoconf::ActivityConf::Activity::Chapter* FindFirstNamedChapter(std::string chapter_name) const;

 private:
  Index_NamedChapterMap index_named_chapter_map_;

};

class ChapterConf : public Messager {
 public:
  static const std::string& Name() { return kProtoName; };
  virtual bool Load(const std::string& dir, Format fmt, const LoadOptions* options = nullptr) override;
  const protoconf::ChapterConf& Data() const { return data_; };

 public:
  const protoconf::ChapterConf::Chapter* Get(uint64_t key1) const;

 private:
  static const std::string kProtoName;
  protoconf::ChapterConf data_;
};

class ThemeConf : public Messager {
 public:
  static const std::string& Name() { return kProtoName; };
  virtual bool Load(const std::string& dir, Format fmt, const LoadOptions* options = nullptr) override;
  const protoconf::ThemeConf& Data() const { return data_; };

 public:
  const protoconf::ThemeConf::Theme* Get(std::string key1) const;

 private:
  static const std::string kProtoName;
  protoconf::ThemeConf data_;
};

}  // namespace tableau

namespace protoconf {
// Here are some type aliases for easy use.
using ActivityConfMgr = tableau::ActivityConf;
using ChapterConfMgr = tableau::ChapterConf;
using ThemeConfMgr = tableau::ThemeConf;
}  // namespace protoconf
