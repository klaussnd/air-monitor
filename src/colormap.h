#pragma once

#include <QColor>
#include <stdint.h>

class Colormap
{
public:
   Colormap(const float min_val, const float max_val)
         : m_min_val(min_val)
         , m_max_val(max_val)
   {
   }

   void setMin(const float min_val)
   {
      m_min_val = min_val;
   }
   void setMax(const float max_val)
   {
      m_max_val = max_val;
   }
   QColor color(const float value) const;

protected:
   float m_min_val = 0.0;
   float m_max_val = 1.0;
};
