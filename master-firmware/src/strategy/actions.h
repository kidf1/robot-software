#ifndef STRATEGY_ACTIONS_H
#define STRATEGY_ACTIONS_H

#include <goap/goap.hpp>
#include "state.h"

/** Number of states goap can visit before giving up. Increasing it means a
 * solution is found on more complex problems at the expense of RAM use. */
#define GOAP_SPACE_SIZE 160

namespace actions {

struct IndexArms : public goap::Action<RobotState> {
    bool can_run(const RobotState& state)
    {
        (void)state;
        return true;
    }

    void plan_effects(RobotState& state)
    {
        state.arms_are_indexed = true;
    }
};

struct RetractArms : public goap::Action<RobotState> {
    bool can_run(const RobotState& state)
    {
        return state.arms_are_indexed;
    }

    void plan_effects(RobotState& state)
    {
        state.arms_are_deployed = false;
    }
};

struct TakePuck : public goap::Action<RobotState> {
    size_t puck_id;

    TakePuck(size_t id)
        : puck_id(id)
    {
    }
    bool can_run(const RobotState& state)
    {
        return !state.arms_are_deployed && state.puck_available[puck_id] && !state.has_puck;
    }

    void plan_effects(RobotState& state)
    {
        state.puck_available[puck_id] = false;
        state.has_puck = true;
        state.has_puck_color = pucks[puck_id].color;
        state.arms_are_deployed = true;
    }
};

struct DepositPuck : public goap::Action<RobotState> {
    size_t zone_id;
    size_t pucks_in_area{0};

    DepositPuck(size_t id)
        : zone_id(id)
    {
    }
    bool can_run(const RobotState& state)
    {
        return (pucks_in_area < 2) && state.has_puck && (state.has_puck_color == areas[zone_id].color);
    }

    void plan_effects(RobotState& state)
    {
        state.classified_pucks[areas[zone_id].color]++;
        state.has_puck = false;
        state.arms_are_deployed = true;
    }
};

struct LaunchAccelerator : public goap::Action<RobotState> {
    bool can_run(const RobotState& state)
    {
        return !state.accelerator_is_done && !state.arms_are_deployed;
    }

    void plan_effects(RobotState& state)
    {
        state.accelerator_is_done = true;
        state.arms_are_deployed = true;
    }
};

struct TakeGoldonium : public goap::Action<RobotState> {
    bool can_run(const RobotState& state)
    {
        return state.accelerator_is_done && !state.arms_are_deployed && state.goldonium_in_house;
    }

    void plan_effects(RobotState& state)
    {
        state.has_goldonium = true;
        state.goldonium_in_house = false;
        state.arms_are_deployed = true;
    }
};

struct StockPuckInStorage : public goap::Action<RobotState> {
    uint8_t puck_position = 0;
    bool can_run(const RobotState& state)
    {
        if (state.has_puck){
            size_t num_slots = sizeof(state.storage_right) / sizeof(PuckColor);
            for (size_t i = 0; i < num_slots; i++)
            {
                if (state.storage_right[i] == PuckColor_EMPTY)
                {
                    puck_position = i;
                    return true;
                }
            }
        }
        return false;
    }

    void plan_effects(RobotState& state)
    {
        state.storage_right[puck_position] = state.has_puck_color;
        state.has_puck = false;
        state.arms_are_deployed = true;
    }
};

struct PutPuckInScale : public goap::Action<RobotState> {
    uint8_t puck_position = 0;
    bool can_run(const RobotState& state)
    {
        if (state.has_puck){
            size_t num_slots = sizeof(state.puck_in_scale) / sizeof(PuckColor);
            for (size_t i = 0; i < num_slots; i++)
            {
                if (state.puck_in_scale[i] == PuckColor_EMPTY)
                {
                    puck_position = i;
                    return true;
                }
            }
        }
        return false;
    }

    void plan_effects(RobotState& state)
    {
        state.arms_are_deployed = true;
        state.has_puck = false;
        state.puck_in_scale[puck_position] = state.has_puck_color;
    }
};

} // namespace actions

#endif /* STRATEGY_ACTIONS_H */
