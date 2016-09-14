
//
// INF4710 TP1 A2016 v1.0
// Auteurs:
//     @@@@@@@@@@@@@@@@@@@@@@@
//     @@@@@@@@@@@@@@@@@@@@@@@
//

#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

struct LZ77Code {
    uint8_t nIdx; // in-dictionary index offset for first match
    uint8_t nLength; // total consecutive match count
    uint8_t nNextSymbol; // next unmatched symbol
    inline std::string to_string() const {
        return std::string("(")+std::to_string(nIdx)+","+std::to_string(nLength)+",'"+std::to_string(nNextSymbol)+"'),";
    }
};

inline std::vector<LZ77Code> lz77_encode(const std::vector<uint8_t>& vSignal, size_t N, size_t n1) {
    CV_Assert(N>0 && n1>0 && N>n1 && n1<UCHAR_MAX && !vSignal.empty());
    std::vector<LZ77Code> vCode;
    // ... @@@@@ TODO (encode vSignal using lz77, and put triplets in vCode)

	// N = taille total de la fenetre glissante
	// n1 = taille du buffer gauche de la fenetre glissante
	size_t n2 = N - n1; // taille du buffer droit de la fenetre glissante
	std::vector<uint8_t> vSignalCopy = vSignal; // copie du vector du signal
	std::vector<uint8_t> vWindow = std::vector<uint8_t>(); 
	// fenetre gauche rempli de 0
	for (size_t i = 0; i < n1; ++i) {
		vWindow.push_back(0);
	}
	// fenetre de droit rempli du signal
	for (size_t i = 0; i < n2; ++i) {
		vWindow.push_back(vSignalCopy.back());
		vSignalCopy.pop_back();
	}

	while (!vSignalCopy.empty) {
		LZ77Code next = LZ77Code();

		//lire prochaine valeur de la fenetre droite
		uint8_t val = vWindow[n1];
		//chercher index la valeur dans la fenetre a reculons
		next.nIdx = 0;
		while(vWindow[n1 - next.nIdx] != val) {
			next.nIdx++;
			if (next.nIdx >= n1) {
				// pas trouve
				next.nIdx = 0;
				break;
			}
		}
		
		//tant que fenetre(index+1) == vecteur(index1), incrementer length
		while (vWindow[n1 - (next.nIdx-next.nLength)] == vWindow[n1 + next.nLength] && next.nLength < n2) {
			next.nLength++;
		}
		// lz77code.nNextSymbol = derniere valeur lu du vecteur
		if (next.nLength < n2) {
			next.nNextSymbol = vWindow[n1 + next.nLength];
		}
		else {
			next.nNextSymbol = 
		}
		// prendre length valeurs du vecteur et mettre dans fenetre

		vCode.push_back(next);
	}
	
    return vCode;
}

inline std::vector<uint8_t> lz77_decode(const std::vector<LZ77Code>& vCode, size_t N, size_t n1) {
    CV_Assert(N>0 && n1>0 && N>n1 && n1<UCHAR_MAX && !vCode.empty());
    std::vector<uint8_t> vSignal;

	// N = taille total de la fenetre glissante
	// n1 = taille du buffer gauche de la fenetre glissante
    // ... @@@@@ TODO (decode vCode triplets using lz77, and put original values in vSignal)
    return vSignal;
}

inline std::vector<uint8_t> format_signal(const cv::Mat& oInputImage) {
    CV_Assert(!oInputImage.empty() && oInputImage.isContinuous() && (oInputImage.type()==CV_8UC1 || oInputImage.type()==CV_8UC3));
    std::vector<uint8_t> vSignal;
    // ... @@@@@ TODO (put oInputImage data in vSignal in correct order/format)
    return vSignal;
}

inline cv::Mat reformat_image(const std::vector<uint8_t>& vSignal, const cv::Size& oOrigImageSize) {
    CV_Assert(!vSignal.empty() && oOrigImageSize.area()>0 && (oOrigImageSize.area()==vSignal.size() || oOrigImageSize.area()*3==vSignal.size())); // output can only be 1-channel (CV_8UC1) or 3-channel (CV_8UC3)
    if(oOrigImageSize.area()==vSignal.size()) // if the image is single-channel (CV_8UC1), reassemble inline as cv::Mat data is row-major
        return cv::Mat(oOrigImageSize,CV_8UC1,(void*)vSignal.data()).clone();
    std::vector<cv::Mat> voChannels(3); // otherwise, use cv::merge to reinterlace individual CV_8UC1 matrices into a CV_8UC3 one
    for(size_t c=0; c<3; ++c)
        voChannels[c] = cv::Mat(oOrigImageSize,CV_8UC1,(void*)(vSignal.data()+c*oOrigImageSize.area())); // no need for a clone here, we will not modify the original data
    cv::Mat oOrigImage; // will be created/sized automatically by cv::merge
    cv::merge(voChannels,oOrigImage);
    return oOrigImage;
}
