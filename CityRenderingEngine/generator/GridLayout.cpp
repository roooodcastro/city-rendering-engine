#include "GridLayout.h"

GridLayout::GridLayout(const int layoutId, const Vector2 &posMin, const Vector2 &posMax) : GRID_LAYOUT_ID(layoutId) {
    this->posMin = posMin;
    this->posMax = posMax;
}