#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <filesystem>
#include <igl/stb/read_image.h>
#include <igl/stb/write_image.h>
#include "image.h"
#include "line_segments.h"
#include <iomanip>

namespace fs = std::filesystem; // Alias for easier reference to filesystem

int main(int argc, char *argv[])
{
    using namespace Eigen;
    using namespace std;
    using namespace Image;

    // Parse command line inputs
    string input_file;
    string dest_file;
    string dest_folder;
    const char *yellowColor = "\033[33m";
    const char *resetColor = "\033[0m";

    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "--in" && i + 1 < argc)
        {
            input_file = argv[++i];
        }
        else if (arg == "--dest" && i + 1 < argc)
        {
            dest_file = argv[++i];
        }
        else if (arg == "--out" && i + 1 < argc)
        {
            dest_folder = argv[++i];
        }
        else
        {
            std::cerr << yellowColor << "Invalid argument: " << arg << resetColor << std::endl;
            return 1;
        }
    }

    if (input_file.empty() || dest_file.empty() || dest_folder.empty())
    {
        std::cerr << yellowColor << "Usage: " << argv[0] << " --in input.jpg --dest destination.jpg --out destination_folder" << resetColor << std::endl;
        return 1;
    }

    // Ensure destination folder exists, or create it
    if (!fs::exists(dest_folder))
    {
        if (!fs::create_directories(dest_folder))
        {
            std::cerr << yellowColor << "Error: Failed to create destination folder " << dest_folder << resetColor << std::endl;
            return 1;
        }
    }

    // Read source image
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> source_R, source_G, source_B, source_Alpha;
    if (!igl::stb::read_image(input_file, source_R, source_G, source_B, source_Alpha))
    {
        std::cerr << yellowColor << "Error: Failed to read the source image from " << input_file << resetColor << std::endl;
        return 1;
    }

    // Read destination image
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> destination_R, destination_G, destination_B, destination_Alpha;
    if (!igl::stb::read_image(dest_file, destination_R, destination_G, destination_B, destination_Alpha))
    {
        std::cerr << yellowColor << "Error: Failed to read the destination image from " << dest_file << resetColor << std::endl;
        return 1;
    }

    // TODO: Define the line segment pairs for morphing, these numbers are placeholders
    OrientedLineSegmentPairs olsp;
    const Eigen::MatrixXd F_src = (Eigen::MatrixXd(2, 4) << 
    103, 90, 133, 90,
    118, 114, 118, 205
    ).finished();

    const Eigen::MatrixXd F_dest = (Eigen::MatrixXd(2, 4) << 
    97, 55, 141, 45,
    105, 71, 118, 144
    ).finished();

    Eigen::MatrixXd src = F_src, dst = F_dest;
    for (int i = 0; i < src.rows(); ++i)
    {
        Eigen::Vector2d p1_src(src(i, 0), src(i, 1)), p2_src(src(i, 2), src(i, 3));
        Eigen::Vector2d p1_dst(dst(i, 0), dst(i, 1)), p2_dst(dst(i, 2), dst(i, 3));
        olsp.push_back(std::make_pair(OrientedLineSegment(p1_src, p2_src), OrientedLineSegment(p1_dst, p2_dst)));
    }

    // Prepare image processor
    ImageProcessor imgProcessor;

    // Loop over 30 frames, saving each one
    for (int FrameNum = 0; FrameNum <= 30; ++FrameNum)
    {
        float timeStep = FrameNum / 30.0f;

        // Perform morphing for each color channel
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> morphed_R = source_R;
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> morphed_G = source_G;
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> morphed_B = source_B;

        imgProcessor.BeierNeelyMorph(source_R, destination_R, olsp, timeStep, morphed_R);
        imgProcessor.BeierNeelyMorph(source_G, destination_G, olsp, timeStep, morphed_G);
        imgProcessor.BeierNeelyMorph(source_B, destination_B, olsp, timeStep, morphed_B);

        // Generate output filename based on time step
        stringstream output_path;
        output_path << dest_folder << "/output_" << setw(2) << setfill('0') << FrameNum << ".jpg"; // "output_00.jpg", "output_01.jpg", etc.

        if (!igl::stb::write_image(output_path.str(), morphed_R, morphed_G, morphed_B, source_Alpha))
        {
            std::cerr << yellowColor << "Error: Failed to write the output image to " << output_path.str() << resetColor << std::endl;
            return 1;
        }

        std::cout << "Morphing result saved to: " << output_path.str() << std::endl;
    }

    return 0;
}
