#include <sstream>
#include "action_sequence.hpp"
#include "action_type.hpp"

namespace Poker {

ActionSequence::ActionSequence() : sequence(vector<Line>(4)) {}

ActionSequence::ActionSequence(const ActionSequence &p) {
  sequence = p.sequence;
}

ActionSequence::~ActionSequence() {}

void ActionSequence::append(Action action, PhaseType::Enum phase) {
  sequence[phase].push_back(action);
}

ActionSequence ActionSequence::assume(Action action,
                                      PhaseType::Enum phase) const {
  ActionSequence new_seq = ActionSequence(*this);
  new_seq.append(action, phase);
  return new_seq;
}

ActionSequence
ActionSequence::subtract(const ActionSequence &subsequence) const {
  ActionSequence new_seq = ActionSequence();
  for (int i = 0; i < 4; ++i) {
    PhaseType::Enum phase = static_cast<PhaseType::Enum>(i);
    for (unsigned a = 0; a < sequence[i].size(); ++a) {
      Action mainaction = sequence[i][a].action;

      // if subsequence is smaller than i in this phase, insert every action
      // past
      if (subsequence.sequence[i].size() < (a + 1)) {
        new_seq.append(mainaction, phase);
        continue;
      }

      Action subaction = subsequence.sequence[i][a].action;

      if (subaction != mainaction) {
        new_seq.append(mainaction, phase);
      }
    }
  }
  return new_seq;
}

string ActionSequence::to_str() const {
  std::stringstream ss;
  for (int i = 0; i < 4; i++) {
    for (unsigned a = 0; a < sequence[i].size(); a++) {
      ss << ActionType::ToStrShort[sequence[i][a].action];
    }
    if (i != 3)
      ss << "/";
  }
  return ss.str();
}
};
