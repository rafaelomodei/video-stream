#include "VideoStreamHandler.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

VideoStreamHandler::VideoStreamHandler(int cameraIndex, const std::function<void(const std::string &)> &sendFrameCallback)
    : cameraIndex(cameraIndex), sendFrameCallback(sendFrameCallback), isStreaming(false) {}

VideoStreamHandler::~VideoStreamHandler() {
  stopStreaming();
}

void VideoStreamHandler::startStreaming() {
  if (isStreaming)
    return;

  isStreaming = true;
  streamingThread = std::thread(&VideoStreamHandler::streamLoop, this);
}

void VideoStreamHandler::stopStreaming() {
  if (isStreaming) {
    isStreaming = false;
    if (streamingThread.joinable()) {
      streamingThread.join();
    }
  }
}

void VideoStreamHandler::streamLoop() {
  cv::VideoCapture cap(cameraIndex);

  // Configuração adicional
  cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  cap.set(cv::CAP_PROP_FPS, 30);

  if (!cap.isOpened()) {
    std::cerr << "Erro ao abrir a câmera no índice: " << cameraIndex << std::endl;
    return;
  }

  std::cout << "Câmera aberta com sucesso! Índice: " << cameraIndex << std::endl;

  cv::Mat frame;
  while (isStreaming) {
    cap >> frame;
    if (frame.empty()) {
      std::cerr << "Frame vazio capturado, continuando..." << std::endl;
      continue;
    }

    cv::flip(frame, frame, 0);

    // Converte o frame para JPEG
    std::vector<unsigned char> buffer;
    cv::imencode(".jpg", frame, buffer);

    // Converte para string e envia via callback
    std::string frameBase64(buffer.begin(), buffer.end());
    if (sendFrameCallback) {
      try {
        sendFrameCallback(frameBase64);
      } catch (const std::exception &e) {
        std::cerr << "Erro ao enviar frame: " << e.what() << std::endl;
      }
    }

    // Limitar taxa de quadros
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  cap.release();
}
