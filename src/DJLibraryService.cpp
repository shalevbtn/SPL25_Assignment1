#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    for(SessionConfig::TrackInfo ti : library_tracks){
        AudioTrack* at;
        if (ti.type == "MP3"){
            at = new MP3Track(ti.title, ti.artists, ti.duration_seconds, ti.bpm, ti.extra_param1, ti.extra_param2);
            std::cout << "MP3Track created: " << ti.extra_param1 << " kbps" << std::endl;
        }
        else{
            at = new WAVTrack(ti.title, ti.artists, ti.duration_seconds, ti.bpm, ti.extra_param1, ti.extra_param2);
            std::cout << "WAVTrack created: " << ti.extra_param1 << "Hz/" << ti.extra_param2 << "bit" << std::endl;
        }
        library.push_back(at);
    }
    std::cout << "[INFO] Track library built: " << library_tracks.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    AudioTrack* at = playlist.find_track(track_title);
    return at; // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    Playlist p(playlist_name);
    for (int i : track_indices){
        if (i < 1 || i > library.size()){
            std::cout << "[WARNING] Invalid track index: " << i << std::endl;
        }
        else{
            AudioTrack* at = library[i - 1];
            PointerWrapper<AudioTrack> pw = (at->clone());
            if (pw.get() == nullptr){
                std::cout<< "error" << std::endl;
            }
            else{
                AudioTrack* at = pw.get();
                at->load();
                at->analyze_beatgrid();
                playlist.add_track(at);
                std::cout << "Added '" << at->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << track_indices.size() << " tracks" << std::endl;
    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> v;

    for(AudioTrack* at: playlist.getTracks()){
        v.push_back(at->get_title());
    }
    
    return v; 
}
