#include "HeightSource.h"

namespace geo {
HeightSource &HeightSource::instance()
{
    static HeightSource src;
    return src;
}

int32_t HeightSource::at(const Coords3D &coord)
{
    if (m_interfaces.empty())
        return 0;
    int32_t out = m_defaultHeight;
    for (HeightSourceInterface *i : m_interfaces)
    {
        if (i->heightAt(coord, m_defaultHeight, out))
        {
            if (!m_hasValidSource) {
                m_hasValidSource = true;
                m_defaultHeight = 0;
            }
            break;
        }
    }
    return out;
}

void HeightSource::setInterface(HeightSourceInterface *i)
{
    m_interfaces.push_back(i);
}

HeightSource::~HeightSource()
{
    while (!m_interfaces.empty())
    {
        delete m_interfaces.front();
        m_interfaces.pop_front();
    }
}

HeightSource::HeightSource() : m_defaultHeight(0), m_hasValidSource(false)
{

}

int32_t HeightSource::defaultHeight() const
{
    return m_defaultHeight;
}

void HeightSource::setDefaultHeight(int32_t newDefaultHeight)
{
    if (m_hasValidSource)
        return;
    m_defaultHeight = newDefaultHeight;
}

void HeightSource::cacheArea(const Coords3D &leftTop, const Coords3D &rightBottom)
{
    for (HeightSourceInterface *i : m_interfaces)
        i->cacheArea(leftTop, rightBottom);
}
}
