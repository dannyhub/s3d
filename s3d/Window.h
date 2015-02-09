#pragma once

#include <string>
#include <boost\noncopyable.hpp>
#include <windows.h>
#include <memory>

namespace s3d
{

class Renderer;

class WindowCreateFailedException : public std::exception {
public:
  WindowCreateFailedException(const char * const & msg) : exception(msg) {
  }
};


class Window : private boost::noncopyable {
public:
  virtual ~Window();

  static std::shared_ptr<Window> create();
  void destroy();

  void show();
  void hide();

  void moveTo(int x, int y);
  void setRect(int x, int y, int weigh, int height);
  void getRect(int* x, int* y, int* weigh, int* height);

  void setWindowTitle(const std::string&);
  std::string getWindowTitle() const;

  void needRedraw();

  Renderer* renderer() {
    return renderer_;
  }

  friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
  virtual bool onCreate();
  virtual void onDraw(Renderer& renderer);
  virtual void onClose();

  virtual void onUpdate() {}
private:
  Window();
  LRESULT wndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
  Renderer* renderer_;
  HWND hWnd_;
};

}// namespace s3d