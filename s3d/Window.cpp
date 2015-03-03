#include "stdafx.h"
#include "Window.h"
#include "Renderer.h"

#include <unordered_map>

#include "Object.h"
#include "Camera.h"
#include "PLGLoader.h"

using namespace std;


namespace s3d
{

namespace 
{
unordered_map<HWND, shared_ptr<Window>> g_hwndWindowMap;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  if (hWnd == NULL) {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  if (message == WM_CREATE)
    {
         CREATESTRUCT*  cp = (CREATESTRUCT*)lParam;
         Window*  wnd = static_cast<Window*>(cp->lpCreateParams);
         wnd->hWnd_ = hWnd;
         assert(wnd);
         shared_ptr<Window> WindowPtr(wnd);
         if (WindowPtr->onCreate()) {
           g_hwndWindowMap.insert(make_pair(wnd->hWnd_, WindowPtr));
          return 0;
         }

         return -1;
    }

  auto it = g_hwndWindowMap.find(hWnd);
  if (it != g_hwndWindowMap.end()) {
    auto wnd = it->second;
    return wnd->wndProc(message, wParam, lParam);
  }

  return DefWindowProc(hWnd, message, wParam, lParam);

}

Window::Window() : hWnd_(NULL){
}

Window::~Window() {
  assert(hWnd_ == NULL);
}

static double anglex = -0 * 3.1415927 / 180;;
static double angley = 0 * 3.1415927 / 180;
static double anglez = 0 * 3.1415927 / 180;

double wx = 0;
double wy = 0;
double wz = 100;

double cx = 0;
double cy = 0;
double cz = 0;

LRESULT Window::wndProc(UINT message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message) {
  case  WM_KEYDOWN:
    if (0x41 == wParam) {
      cx -= 7;
    }

    if (0x44 == wParam) {
      cx += 7;
    }

    if (0x57 == wParam) {
      cy += 7;
    }

    if (0x53 == wParam) {
      cy -= 7;
    }


    //f
    if (0x46 == wParam) {
      cz += 7;
    }

    //g
    if (0x47 == wParam) {
      cz -= 7;
    }


    //i
    if (0x49 == wParam) {
      anglex += 7 * 3.1415927 / 180.0;
    }

    //k
    if (0x4B == wParam) {
      anglex -= 7 * 3.1415927 / 180.0;
    }

    //j
    if (0x4A == wParam) {
      angley -= 7 * 3.1415927 / 180.0;
    }

    //l
    if (0x4C == wParam) {
      angley += 7 * 3.1415927 / 180.0;
    }

    //0
    if (0x4F == wParam) {
      anglez -= 7 * 3.1415927 / 180.0;
    }

    //p
    if (0x50 == wParam) {
      anglez += 7 * 3.1415927 / 180.0;
    }


        //anglex += 1 * 3.1415927 / 180;
    //cz += 10;
    this->needRedraw();
    return DefWindowProc(hWnd_, message, wParam, lParam);;
  break;
  case WM_ERASEBKGND:
    return TRUE;
  break;

  case WM_PAINT:
    hdc = BeginPaint(hWnd_, &ps);
    {
      HDC hMemDC = CreateCompatibleDC(hdc);
      RECT rect;
      ::GetWindowRect(hWnd_, &rect);
      const int winWidth = rect.right - rect.left + 1;
      const int winHeight = rect.bottom - rect.top + 1;

      BITMAPINFO bmi;
      memset(&bmi, 0, sizeof(BITMAPINFO));
      bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth = winWidth;
      bmi.bmiHeader.biHeight = -winHeight; // top-down
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = 32;
      bmi.bmiHeader.biCompression = BI_RGB;

      uint32_t* buffer = NULL;
      HBITMAP hBmp = CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS, (void**)&buffer, NULL, NULL);
      std::fill(buffer, buffer + winWidth * winHeight, RGB(0, 0, 0));

      s3d::Renderer renderer(buffer, winWidth, winHeight);
      try {
        this->onDraw(renderer);
      } catch (...) {
      }
      
      HGDIOBJ oldObj = SelectObject(hMemDC, hBmp);
      BitBlt(hdc, 0, 0, winWidth, winHeight, hMemDC, 0, 0, SRCCOPY);

      SelectObject(hMemDC, oldObj);
      DeleteObject(hMemDC);
      DeleteObject(hBmp);

      SelectObject(hMemDC, GetStockObject(DC_PEN));
      SetDCPenColor(hMemDC, RGB(0, 0, 255));
    }

    EndPaint(hWnd_, &ps);
    break;

  case WM_CLOSE:
  {
    auto it = g_hwndWindowMap.find(hWnd_);
     if (it != g_hwndWindowMap.end()) {
       it->second->destroy();
       return 0;
     } else {
       assert(false);
     }
  }break;
  case WM_DESTROY:
  {
     auto it = g_hwndWindowMap.find(hWnd_);
     if (it != g_hwndWindowMap.end()) {
       g_hwndWindowMap.erase(it);
     } else {
       assert(false);
     }

     if (g_hwndWindowMap.empty()) {
       PostQuitMessage(0);
     }
  }break;
  default:
    return DefWindowProc(hWnd_, message, wParam, lParam);
  }
  return 0;
}

bool Window::onCreate() {
  return true;
}

void Window::needRedraw() {
  RECT rect;
  ::GetWindowRect(hWnd_, &rect);
  ::InvalidateRect(NULL, NULL, FALSE);
}



void Window::onDraw(Renderer& renderer) {
  // for (int i = 0; i<1000; ++i)
  // renderer.drawLine2D_DDA(Point2<int>(rand() % 1024, rand() % 1024), Point2<int>(rand() % rand() % 1024, 230), Color(rand() % 255, rand() % 255, rand() % 255));
  RECT rect;
  ::GetWindowRect(hWnd_, &rect);
  const int winWidth = rect.right - rect.left + 1;
  const int winHeight = rect.bottom - rect.top + 1;

  Object obj(1, "testobj");

  PLGLoader plgloader;
  try {
    std::string name; 
    VertexList<Point4<double>> vlist;
    std::vector<Polygon<3>> polys;
    plgloader.parse("D:\\work\\t3dlib\\T3DIICHAP07\\tank2.plg", name, vlist, polys);

    for (auto pt : vlist) {
      obj.addVertex(pt);
    }

    for (auto poly : polys) {
      obj.addPolygon(poly);
    }

  } catch (...) {
  }
  //obj.addVertex({5, 5, 5}); // p0
  //obj.addVertex({-5, 5, 5}); // p1
  //obj.addVertex({-5, 5, -5});// p2
  //obj.addVertex({5, 5, -5}); // p3
  //obj.addVertex({5, -5, 5}); // p4
  //obj.addVertex({-5, -5, 5}); // p5
  //obj.addVertex({-5, -5, -5}); // p6
  //obj.addVertex({5, -5, -5});// p7

  //obj.addVertex({0, 40, 30});
  //obj.addVertex({40, -40, 30});
  //obj.addVertex({-40, -40, 30});

  obj.addVertex({-10, 0, 0});
  obj.addVertex({10, 0, 0});

  obj.addVertex({0, -10, 0});
  obj.addVertex({0, 10, 0});

  obj.addVertex({0, 0, 10});
  obj.addVertex({0, 0, -10});

  unsigned int temp_poly_indices[12 * 3] = {
    0, 1, 2, 0, 2, 3, // polygons 0 and 1
    0, 7, 4, 0, 3, 7, // polygons 2 and 3
    4, 7, 6, 4, 6, 5, // polygons 4 and 5
    1, 6, 2, 1, 5, 6, // polygons 6 and 7
    3, 6, 7, 3, 2, 6, // polygons 8 and 9
    0, 4, 5, 0, 1, 5}; // polygons 10 and 11

  //for (int i = 0; i < 1/*12 * 3*/; i += 3) {
  //  int id = temp_poly_indices[i];
  //  Point2<int> p0 = {(int)obj.localVertexList_[id].x_, (int)obj.localVertexList_[id].y_};

  //  id = temp_poly_indices[i + 1];
  //  Point2<int> p1 = {(int)obj.localVertexList_[id].x_, (int)obj.localVertexList_[id].y_};

  //  id = temp_poly_indices[i + 2];
  //  Point2<int> p2 = {(int)obj.localVertexList_[id].x_, (int)obj.localVertexList_[id].y_};
  //  
  //  obj.addPolygon({temp_poly_indices[i], temp_poly_indices[i+1], temp_poly_indices[i+2]});
  //}

  auto rotateMat = buildRotateMatrix4x4(-anglex, -angley, -anglez);
  for (auto& v : obj.localVertexList_) {
    v = (v * rotateMat);
  }

  //for (auto& itp : obj.polygons_) {
  //  itp.normal_ = itp.normal_ * rotateMat;
  //  //itp.normal_.normalizeSelf();
  //}
 
  addToWorld(obj, wx, wy, wz);


  int viewWidth = winWidth;
  int viewHeight = winHeight;

  auto camera = createUVNCamera({cx, cy, cz - 310}, {cx, cy, cz + 300}, 90, 1, 1000, viewWidth, winHeight);

  for (auto& itp : obj.polygons_) {
    const auto u = obj.transVertexList_[itp.at(1)] - obj.transVertexList_[itp.at(0)];
    const auto v = obj.transVertexList_[itp.at(2)] - obj.transVertexList_[itp.at(0)];

    itp.normal_ = u.crossProduct(v);
    Vector4FD vp = camera->getPosition() - obj.transVertexList_[itp.at(0)];
    if ( !(itp.getState() & kPolygonState2Side) && vp.dotProduct(itp.normal_) > 0.) {
      itp.setState(kPolygonStateVisible);
    }
  }



  auto mat = camera->getWorldToScreenMatrix4x4FD();

  auto transVerit = obj.transVertexList_.begin();
  while (transVerit != obj.transVertexList_.end()) {
    const auto pt = *transVerit;
    *transVerit = pt * mat;
    ++transVerit;
  }


  //setCamera(obj, {cx, cy, cz}, -0, -0, -0);

  //mat = camera->getCameraToProjectMatrix4x4FD();

  //transVerit = obj.transVertexList_.begin();
  //while (transVerit != obj.transVertexList_.end()) {
  //  const auto pt = *transVerit;
  //  *transVerit = pt * mat;
  //  ++transVerit;
  //}

  for (auto& itp : obj.polygons_) {
    if (itp.getState() & kPolygonStateVisible) {
        obj.transPolygons_.push_back(itp);
    }
  }
  //perspectiveProject(obj, viewWidth, viewHeight);
  //perspectiveProject(obj, 90, viewWidth, viewHeight);

  for (auto itp : obj.transPolygons_) {
    int id = itp[0];
    Point2<int> p0 = {(int)obj.transVertexList_[id].x_, (int)obj.transVertexList_[id].y_};

    id = itp[1];
    Point2<int> p1 = {(int)obj.transVertexList_[id].x_, (int)obj.transVertexList_[id].y_};

    id = itp[2];
    Point2<int> p2 = {(int)obj.transVertexList_[id].x_, (int)obj.transVertexList_[id].y_};
    renderer.drawTriangle2D(p0, p1, p2, Color(0xc0, 0xf0, 0));
  }


  //int linestart = 8;
  //renderer.drawLine2D({(int)obj.transVertexList_[linestart].x_, (int)obj.transVertexList_[linestart].y_}, {(int)obj.transVertexList_[linestart + 1].x_, (int)obj.transVertexList_[linestart+1].y_}, Color(255, 0, 0));
  //
  //linestart = 10;
  //renderer.drawLine2D({(int)obj.transVertexList_[linestart].x_, (int)obj.transVertexList_[linestart].y_}, {(int)obj.transVertexList_[linestart + 1].x_, (int)obj.transVertexList_[linestart + 1].y_}, Color(255, 0, 0));

  //linestart = 12;
  //renderer.drawLine2D({(int)obj.transVertexList_[linestart].x_, (int)obj.transVertexList_[linestart].y_}, {(int)obj.transVertexList_[linestart + 1].x_, (int)obj.transVertexList_[linestart + 1].y_}, Color(0, 255, 0));


  
  //renderer.drawLine2D_Horizontal(Point2<int>(winWidth / 4, winHeight / 2), Point2<int>(winWidth / 2, winHeight / 2), Color(255, 0, 0));
  //renderer.drawLine2D_Horizontal(Point2<int>(winWidth / 8, winHeight / 3), Point2<int>(winWidth / 2, winHeight / 3), Color(255, 0, 0));

  //renderer.drawLine2D_Vertical(Point2<int>(winWidth / 5, winHeight / 4), Point2<int>(winWidth / 5, winHeight / 2), Color(255, 255, 0));
  //renderer.drawLine2D_Vertical(Point2<int>(winWidth / 2, winHeight / 6), Point2<int>(winWidth / 2, winHeight / 3), Color(255, 0, 255));

  //int randW = winWidth - 100;
  //int randH = winHeight - 100;

  //{
  //  Point2<int> p0(rand() % randW, rand() % randH);
  //  Point2<int> p1(rand() % randW, rand() % randH);
  //  Point2<int> p2(rand() % randW, rand() % randH);

  //  renderer.drawTriangle2D(p0, p1, p2, Color(255, 0, 255));
  //}

  //renderer.drawLine2D({0,250}, {1500,250}, Color(255, 0, 0));

  //renderer.drawLine2D({250, 0}, {250, 1700}, Color(255, 0, 0));

  //renderer.drawLine2D({10, 10}, {1700, 1700}, Color(255, 0, 0));

  //{
  //  Point2<int> p0(rand() % randW, rand() % randH);
  //  Point2<int> p1(rand() % randW, rand() % randH);


  //  p0.y_ += 50;
  //  p1.y_ += 50;
  //  renderer.drawLine2D_DDA(p0, p1, Color(255, 0, 0));

  //  p0.y_ += 50;
  //  p1.y_ += 50;
  //  renderer.drawLine2D_Bresenham(p0, p1, Color(255, 0, 0));


  //  p0.y_ -= 100;
  //  p1.y_ -= 100;
  //}



  //POINT lastPt;
  //::MoveToEx(hdc, p0.x_, p0.y_, &lastPt);
  //::LineTo(hdc, p1.x_, p1.y_);
  // Rectangle(hdc, 100, 300, 200, 400);
}


void Window::onClose() {
  this->destroy();
}

std::shared_ptr<Window> Window::create() {
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = NULL;
  wcex.hIcon = NULL;
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName =NULL;
  wcex.lpszClassName = L"S3DClass";
  wcex.hIconSm = NULL;

  ::RegisterClassEx(&wcex);

  Window*  wnd(new Window);
  wnd->hWnd_ = ::CreateWindow(wcex.lpszClassName, L"S3DClass", WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, wcex.hInstance, static_cast<LPVOID>(wnd));
  if (wnd->hWnd_ == NULL) {
    delete wnd;
    throw WindowCreateFailedException("CreateWindow failed hWnd_ == NULL");
  }

  auto it = g_hwndWindowMap.find(wnd->hWnd_);
  if (it != g_hwndWindowMap.end()) {
    return it->second;
  }

  throw WindowCreateFailedException("CreateWindow failed g_hwndWindowMap.find(wnd->hWnd_)");
}

void Window::destroy() {
  if (hWnd_ != NULL) {
    ::DestroyWindow(hWnd_);
    hWnd_ = NULL;
  } 
}

void Window::show() {
  ShowWindow(hWnd_, SW_SHOW);
  UpdateWindow(hWnd_);
}

void Window::hide() {
  ShowWindow(hWnd_, SW_HIDE);
  UpdateWindow(hWnd_);
}

}// namespace s3d