#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <iterator>

#include "sc2api/sc2_api.h"
#include "sc2api/sc2_args.h"
#include "sc2lib/sc2_lib.h"
#include "sc2utils/sc2_manage_process.h"
#include "sc2utils/sc2_arg_parser.h"

using namespace sc2;

#include "helpers.h"
// Helper functions

class Bot : public sc2::Agent {
public:

    virtual void OnGameStart() final {
        staging_location_ = Observation()->GetStartLocation();
    }
    
    virtual void OnStep() final {
        ManageWorkers(UNIT_TYPEID::TERRAN_SCV, ABILITY_ID::HARVEST_GATHER, UNIT_TYPEID::TERRAN_REFINERY);
        TryBuildBarracks();
        TryBuildSupplyDepot();
        TryBuildRefinery();
        if (Observation()->GetMinerals() > 150 && Observation()->GetVespene() > 100) {
            TryBuildStructureRandom(ABILITY_ID::BUILD_FACTORY, UNIT_TYPEID::TERRAN_SCV);
        }
        if (CountUnitType(UNIT_TYPEID::TERRAN_MARINE) >= 20) {
            TargetSCVCount = 5;
        }
    }

    virtual void OnUnitIdle(const Unit* unit) final {
        switch (unit->unit_type.ToType()) {
            case UNIT_TYPEID::TERRAN_COMMANDCENTER: {
                Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_SCV);
                break;
            }
            case UNIT_TYPEID::TERRAN_SCV: {
                const Unit* mineral_target = FindNearestMineralPatch(unit->pos);
                if (!mineral_target) {
                    break;
                }
                Actions()->UnitCommand(unit, ABILITY_ID::SMART, mineral_target);
                break;
            }
            case UNIT_TYPEID::TERRAN_BARRACKS: {
                Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MARINE);
                /*
                if (CountUnitType(UNIT_TYPEID::TERRAN_MARINE) % 40 <= 30) {
                    Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MARINE);
                }
                else {
                    Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_REAPER);
                }*/
                break;
            }
            case UNIT_TYPEID::TERRAN_MARINE: {
                const ObservationInterface* observation = Observation();
                if (CountUnitType(UNIT_TYPEID::TERRAN_MARINE) >= 20) {
                    Units enemy_units = observation->GetUnits(Unit::Alliance::Enemy);
                    if (enemy_units.empty()) {
                        break;
                    }
                    if (unit->orders.front().ability_id == ABILITY_ID::ATTACK) {
                        float distance = 10000;
                        for (const auto& u : enemy_units) {
                            float d = Distance2D(u->pos, unit->pos);
                            if (d < distance) {
                                distance = d;
                            }
                        }
                        if (distance < 6) {
                            Actions()->UnitCommand(unit, ABILITY_ID::EFFECT_STIM);
                            break;
                        }
                    }
                    AttackWithUnitType(UNIT_TYPEID::TERRAN_MARINE, observation);
                }
                break;
            }/*
            case UNIT_TYPEID::TERRAN_REAPER: {
                const ObservationInterface* observation = Observation();
                if (CountUnitType(UNIT_TYPEID::TERRAN_REAPER) >= 10) {
                    AttackWithUnitType(UNIT_TYPEID::TERRAN_REAPER, observation);
                }
                break;
            }*/
            default: {
                break;
            }
        }
    }
    
    std::vector<Point3D> expansions_;
    Point3D staging_location_;
    

private:
    
#include "BotPrivate.h"

};

int main(int argc, char* argv[]) {
    Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    Bot bot;
    coordinator.SetParticipants({
        CreateParticipant(Race::Terran, &bot),
        CreateComputer(Race::Zerg, Difficulty::Hard)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while (coordinator.Update()) {
    }

    return 0;
}
