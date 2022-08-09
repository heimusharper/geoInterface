#ifndef HEIGHTSOURCE_H
#define HEIGHTSOURCE_H
#include "Coords3D.h"
#include <gdal_priv.h>
#include <cpl_conv.h>
#include "HeightSourceInterface.h"
#include <list>

namespace geo {
class HeightSource
{
public:

    static HeightSource &instance();
    int32_t at(const geo::Coords3D &coord);

    void setInterface(HeightSourceInterface *i);

    ~HeightSource();

    int32_t defaultHeight() const;
    void setDefaultHeight(int32_t newDefaultHeight);

    void cacheArea(const geo::Coords3D &leftTop, const geo::Coords3D &rightBottom);

private:
    HeightSource();

private:

    int32_t m_defaultHeight;
    bool m_hasValidSource;
    std::list<HeightSourceInterface*> m_interfaces;

};
}

#endif // HEIGHTSOURCE_H
