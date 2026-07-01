#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace cv;
using namespace cv::face;
int main(int argc, char** argv) {
    std::string baseDir = "/opt/aicTrain/sMonitor";
    if (argc > 1) baseDir = argv[1];
    std::vector<Mat> images;
    std::vector<int> labels;
    std::ifstream file(baseDir + "/facedata/Faces/at.txt");
    if (!file.is_open()) { std::cerr << "Cannot open at.txt\n"; return 1; }
    std::string line, path, cl;
    int count = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        if (!std::getline(ss, path, ';')) continue;
        if (!std::getline(ss, cl)) continue;
        std::string fullPath = baseDir + "/" + path;
        Mat img = imread(fullPath, IMREAD_GRAYSCALE);
        if (img.empty()) { std::cerr << "Skip: " << fullPath << "\n"; continue; }
        resize(img, img, Size(200, 200));
        images.push_back(img);
        labels.push_back(std::atoi(cl.c_str()));
        count++;
    }
    file.close();
    std::cout << "Loaded " << count << " images\n";
    if (count < 2) { std::cerr << "Not enough\n"; return 1; }
    Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
    model->train(images, labels);
    model->write(baseDir + "/facedata/MyFacePCAModel.xml");
    std::cout << "Model saved!\n";
    return 0;
}
