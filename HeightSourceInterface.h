#ifndef HEIGHTSOURCEINTERFACE_H
#define HEIGHTSOURCEINTERFACE_H
#include "Coords3D.h"

namespace geo {
class HeightSourceInterface
{
public:
    HeightSourceInterface();
    virtual ~HeightSourceInterface();

    virtual bool heightAt(const Coords3D &coord, const int32_t nd, int32_t &out) = 0;
    virtual void cacheArea(const Coords3D &leftTop, const Coords3D &rightBottom) = 0;
};
}
#endif // HEIGHTSOURCEINTERFACE_H
