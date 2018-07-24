#include "StreamHandler.h"
using namespace std;

int main(){
    std::string high_speed_link = "../processed_data/cam_framedata.rawdata";
    std::string low_speed_link = "../processed_data/cam_metadata.rawinfo";
    std::string high_speed_output = "../processed_data/RSH_frameCache.rawdata";
    std::string low_speed_output = "../processed_data/RSH_meteCache.rawinfo";
    std::string final_mlv = "../processed_data/AXIOMBetaVideo.mlv";

    std::thread HSH (high_speed_handler, high_speed_link, high_speed_output);
    std::thread LSH (low_speed_handler, low_speed_link, low_speed_output);

    HSH.join();
    LSH.join();

    system(("cat "+low_speed_output+" "+high_speed_link+" > "+final_mlv).c_str());

    return 0;
}
