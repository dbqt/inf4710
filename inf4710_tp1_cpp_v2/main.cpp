
#include "tp1.hpp"

using ImagePathFlag = std::pair<std::string,int>; // first = image path, second = cv::imread flag

int main(int /*argc*/, char** /*argv*/) {
	/*// Exemple du tp
	std::vector<uint8_t>* signal = new std::vector<uint8_t>();
	//001 010 210 210 212 401 010 2100
	signal->push_back(0);
	signal->push_back(0);
	signal->push_back(1);
	signal->push_back(2);

	signal->push_back(0);
	signal->push_back(1);
	signal->push_back(0);

	signal->push_back(1);
	signal->push_back(0);
	signal->push_back(4);

	signal->push_back(2);
	signal->push_back(1);
	signal->push_back(2);

	signal->push_back(0);
	signal->push_back(1);
	signal->push_back(2);

	signal->push_back(0);
	signal->push_back(1);
	signal->push_back(2);

	signal->push_back(0);
	signal->push_back(1);
	signal->push_back(0);

	signal->push_back(1);
	signal->push_back(0);
	signal->push_back(0);

	for (auto it = signal->end()-1; it != signal->begin(); it--) {
		std::cout << (int)(*it);
	}
	std::cout << std::endl;
	std::vector<LZ77Code> code = lz77_encode(*signal, 18, 9);

	std::vector<uint8_t> signalDecode = lz77_decode(code, 18, 9);*/

	
    try {
        // note: by default, imread always returns 3-ch images unless the cv::IMREAD_GRAYSCALE flag is set (here we hardcode it based on prior knowledge)
        const std::vector<ImagePathFlag> vsTestImages = {
            {"data/test1.png",cv::IMREAD_GRAYSCALE},
            {"data/test2.png",cv::IMREAD_GRAYSCALE},
            {"data/test3.png",cv::IMREAD_GRAYSCALE},
            {"data/test4.png",cv::IMREAD_COLOR},
            {"data/test5.png",cv::IMREAD_COLOR},
            {"data/test6.png",cv::IMREAD_COLOR},
            {"data/test7.png",cv::IMREAD_COLOR},
            {"data/test8.jpg",cv::IMREAD_COLOR},
            {"data/test9.bmp",cv::IMREAD_COLOR},
            {"data/test10.bmp",cv::IMREAD_COLOR},
        };
        for(const ImagePathFlag& oImagePathFlag : vsTestImages) {
            cv::Mat oInputImg = cv::imread(oImagePathFlag.first,oImagePathFlag.second);
            if(oInputImg.empty())
                CV_Error_(-1,("Could not load image at '%s', check local paths",oImagePathFlag.first.c_str()));

            // ... @@@@ TODO (make sure decoding also provides the original image!)
			std::vector<uint8_t> signal = format_signal(oInputImg);
			cv::Mat oOutputImg = reformat_image(signal, oInputImg.size());

			cv::Mat dst = cv::Mat();
			cv::bitwise_xor(oInputImg, oOutputImg, dst);
			if (cv::countNonZero(dst) > 0) { //check non-0 pixels
				//do stuff in case cv::Mat are not the same
				std::cout << "Different" << std::endl;
			}
			else {
				//do stuff in case they are equal
				std::cout << "Not Different" << std::endl;
			}
				
        }
    }
    catch(const cv::Exception& e) {
        std::cerr << "Caught cv::Exceptions: " << e.what() << std::endl;
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Caught std::runtime_error: " << e.what() << std::endl;
    }
    catch(...) {
        std::cerr << "Caught unhandled exception." << std::endl;
    }
    return 0;
}
