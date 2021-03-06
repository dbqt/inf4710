
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

	std::cout << "Start Encode" << std::endl;
	
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

	// tant qu'il reste de quoi a traiter (il reste des valeurs dans la fenetre droite)
	while (vWindow.size() > n1) {
		LZ77Code lz = LZ77Code();
		
		LZ77Code bestLz = LZ77Code();
		
		uint8_t val = vWindow[n1]; //premiere valeur de fenetre droite

		// initialisation des lz77
		lz.nIdx = 1;
		lz.nLength = 0;
		// le meilleur cas de base, c'est de ne pas le trouver
		bestLz.nIdx = 0;
		bestLz.nLength = 0;
		bestLz.nNextSymbol = val;

		// trouver l'index de cette valeur dans le fenetre gauche si elle est presente
		while (lz.nIdx < n1) {

			// si on l'a trouve
			if (vWindow[n1 - lz.nIdx] == val) {
				// determiner quel longueur
				// tant que les valeurs correspondent et qu'on n'est pas a la fin de la fenetre
				while (lz.nLength < n2 && vWindow[n1 - lz.nIdx + lz.nLength] == vWindow[n1 + lz.nLength]) {
					lz.nLength++;
				}

				// on garde la prochaine non-trouve
				lz.nNextSymbol = vWindow[n1 + lz.nLength];

				if (bestLz.nLength < lz.nLength) {
					bestLz = lz;
					lz.nLength = 0;
				}
			}
			lz.nIdx++;
		}

		// on bouge de lz.nLength + 1
		for (uint8_t i = 0; i < bestLz.nLength + 1; ++i) {
			if (!vSignalCopy.empty()) {
				vWindow.push_back(vSignalCopy.back());
				vSignalCopy.pop_back();
			}
			vWindow.erase(vWindow.begin());
		}

		// ajout du triplet
		vCode.push_back(bestLz);
	}
	
    return vCode;
}

inline std::vector<uint8_t> lz77_decode(const std::vector<LZ77Code>& vCode, size_t N, size_t n1) {
    CV_Assert(N>0 && n1>0 && N>n1 && n1<UCHAR_MAX && !vCode.empty());
    std::vector<uint8_t> vSignal;

	// N = taille total de la fenetre glissante
	// n1 = taille du buffer gauche de la fenetre glissante
    // ... @@@@@ TODO (decode vCode triplets using lz77, and put original values in vSignal)
	std::vector<uint8_t> vWindow = std::vector<uint8_t>();

	// fenetre gauche rempli de 0
	for (size_t i = 0; i < n1; ++i) {
		vWindow.push_back(0);
	}

	for (auto it = vCode.begin(); it != vCode.end(); it++) {
		LZ77Code lz = *it;
		// aller a index ( si zero, skip lecture du window)
		if (lz.nIdx != 0) {
			// Ajouter les lenght prochains symboles au dictionnaire et au signal
			for (uint8_t i = 0; i < lz.nLength; ++i) {
				uint8_t value = vWindow[n1 - lz.nIdx];
				vWindow.push_back(value);
				vWindow.erase(vWindow.begin());
				vSignal.push_back(value);
			}
		}
		// ajouter next symbol a la fin (au signal et dans le window)
		uint8_t value = lz.nNextSymbol;
		vWindow.push_back(value);
		vWindow.erase(vWindow.begin());
		vSignal.push_back(value);
	}

    return vSignal;
}

inline std::vector<uint8_t> format_signal(const cv::Mat& oInputImage) {
    CV_Assert(!oInputImage.empty() && oInputImage.isContinuous() && (oInputImage.type()==CV_8UC1 || oInputImage.type()==CV_8UC3));
    std::vector<uint8_t> vSignal;
    // ... @@@@@ TODO (put oInputImage data in vSignal in correct order/format)
	cv::Mat oInputImageCopy = oInputImage;
	if (oInputImageCopy.type() == CV_8UC1)
	{
		vSignal.assign(oInputImageCopy.datastart, oInputImageCopy.dataend);
	}
	else
	{
		std::vector<cv::Mat> voChannels(3);
		
		//cv::split(oInputImageCopy, voChannels);
		//oInputImageCopy.
		/*
		//auto it = vSignal.begin();
		for (size_t i = 0; i < 3; ++i) {
			cv::extractChannel(oInputImageCopy, voChannels.at(i), 0);
			vSignal.insert(it, voChannels.at(i).datastart, voChannels.at(i).dataend);
			it = vSignal.end();
		}
	}*/

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
