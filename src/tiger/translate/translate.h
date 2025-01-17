#ifndef TIGER_TRANSLATE_TRANSLATE_H_
#define TIGER_TRANSLATE_TRANSLATE_H_

#include <list>
#include <memory>

#include "tiger/absyn/absyn.h"
#include "tiger/env/env.h"
#include "tiger/errormsg/errormsg.h"
#include "tiger/frame/frame.h"
#include "tiger/semant/types.h"

namespace tr {

class Exp;
class ExpAndTy;
class Level;

class PatchList {
public:
  void DoPatch(temp::Label *label) {
    for (auto &patch : patch_list_) {
      // if (*patch == nullptr)
      *patch = label;
    }
  }
  temp::Label **GetFront() const {
    if (!patch_list_.empty()) {
      return patch_list_.front();
    }
    return nullptr;
  }
  // Method to add a label pointer to the patch list
  // void AddLabel(temp::Label **labelPtr) { patch_list_.push_back(labelPtr); }
  explicit PatchList(std::list<temp::Label **> patch_list)
      : patch_list_(patch_list) {}
  PatchList() = default;

  [[nodiscard]] const std::list<temp::Label **> &GetList() const {
    return patch_list_;
  }

private:
  std::list<temp::Label **> patch_list_;
};

class Access {
public:
  Level *level_;
  frame::Access *access_;

  Access(Level *level, frame::Access *access)
      : level_(level), access_(access) {}
  static Access *AllocLocal(Level *level, bool escape);
};

class Level {
public:
  frame::Frame *frame_;
  Level *parent_;

  Level() : frame_(nullptr), parent_(nullptr) {}
  explicit Level(frame::Frame *frame) : frame_(frame), parent_(nullptr) {}
  Level(frame::Frame *frame, Level *parent) : frame_(frame), parent_(parent) {}
};

class ProgTr {
public:
  // TODO: Put your lab5 code here */
  ProgTr(std::unique_ptr<absyn::AbsynTree> absyn_tree,
         std::unique_ptr<err::ErrorMsg> errormsg) {
    tenv_ = std::make_unique<env::TEnv>();
    venv_ = std::make_unique<env::VEnv>();
    absyn_tree_ = std::move(absyn_tree);
    errormsg_ = std::move(errormsg);
    main_level_ = std::make_unique<Level>();
    FillBaseVEnv();
    FillBaseTEnv();
  }
  /**
   * Translate IR tree
   */
  void Translate();

  /**
   * Transfer the ownership of errormsg to outer scope
   * @return unique pointer to errormsg
   */
  std::unique_ptr<err::ErrorMsg> TransferErrormsg() {
    return std::move(errormsg_);
  }

private:
  std::unique_ptr<absyn::AbsynTree> absyn_tree_;
  std::unique_ptr<err::ErrorMsg> errormsg_;
  std::unique_ptr<Level> main_level_;
  std::unique_ptr<env::TEnv> tenv_;
  std::unique_ptr<env::VEnv> venv_;

  // Fill base symbol for var env and type env
  void FillBaseVEnv();
  void FillBaseTEnv();
};
void ProcEntryExit(Level *level, Exp *body);
} // namespace tr

#endif
