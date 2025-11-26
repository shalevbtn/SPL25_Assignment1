#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...." << std::endl;
    for(auto& deck : decks) {
        if(deck != nullptr) {
            delete deck;
            deck = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 * TO CHECK
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===\n";

    PointerWrapper<AudioTrack> track_ptr = track.clone(); ;

    if (!track_ptr) {
        std::cerr << "[ERROR] Track: " << track.get_title() << " failed to clone";
        return -1;
    }

    size_t target_deck = 1 - active_deck; 
 
    std::cout << "\n[Deck Switch] Target deck: " << target_deck;
    if(decks[target_deck] != nullptr) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    track_ptr->load();
    track_ptr->analyze_beatgrid();

    if(decks[active_deck] != nullptr && auto_sync && !can_mix_tracks(track_ptr))
        sync_bpm(track_ptr);


    decks[target_deck] = track_ptr.get();
    track_ptr.release();

    std::cout << "\n[Load Complete] " << track.get_title() << " is now loaded on deck " << target_deck;

    if(decks[active_deck] != nullptr) {
        std::cout << "\n[Unload] Unloading previous deck " << active_deck << "(" << decks[active_deck]->get_title() << ")";
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }

    active_deck = target_deck;
    std::cout << "\n[Active Deck] Switched to deck " << target_deck;
    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] == nullptr || track.get() != nullptr)
        return false;

    int bpm_diff = std::abs(decks[active_deck]->get_bpm() - track->get_bpm());

    if(bpm_diff > bpm_tolerance)
        return false;
    
    return true;
    
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] != nullptr && track.get() != nullptr) {
        int track_prev_bpm = track->get_bpm();
        int new_bpm = (decks[active_deck]->get_bpm() + track_prev_bpm) / 2;
        track->set_bpm(new_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << track_prev_bpm << " to " << new_bpm;
    }
}
