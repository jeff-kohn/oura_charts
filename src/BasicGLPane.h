#include "wx/glcanvas.h"

namespace oura_charts
{
   class ChartView;

   class BasicGLPane : public wxGLCanvas
   {
   public:
      BasicGLPane(wxFrame* parent, int* args);
      virtual ~BasicGLPane();

      void resized(wxSizeEvent& evt);

      int getWidth();
      int getHeight();

      void render(wxPaintEvent& evt);
      void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
      void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

      // events
      void mouseMoved(wxMouseEvent& event);
      void mouseDown(wxMouseEvent& event);
      void mouseWheelMoved(wxMouseEvent& event);
      void mouseReleased(wxMouseEvent& event);
      void rightClick(wxMouseEvent& event);
      void mouseLeftWindow(wxMouseEvent& event);
      void keyPressed(wxKeyEvent& event);
      void keyReleased(wxKeyEvent& event);

   private:
      wxGLContext* m_context{};
      ChartView* m_view{};
   };


} // namespace oura_charts
