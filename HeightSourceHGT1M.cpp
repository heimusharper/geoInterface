#include "HeightSourceHGT1M.h"

namespace geo {
HeightSourceHGT1M::HeightSourceHGT1M(const boost::filesystem::path& path)
    : HeightSourceInterface()
    , m_path(path)
{
    GDALAllRegister();
}

bool HeightSourceHGT1M::heightAt(const Coords3D &coord, const int32_t nd, int32_t &out)
{
    RasterObject *ds = getRasterAt(coord);
    out = nd;
    if (ds != nullptr && ds->isValid())
        return ds->valueAtPosition(coord, out);
    return false;
}

void HeightSourceHGT1M::cacheArea(const Coords3D &leftTop, const Coords3D &rightBottom)
{
    double x = leftTop.lon();
    while (x < rightBottom.lon())
    {
        double y = rightBottom.lat();
        while (y < leftTop.lat())
        {
            getRasterAt(geo::Coords3D(y, x)); // cache
            y += 1.;
        }
        x += 1.;
    }
}

HeightSourceHGT1M::RasterObject *HeightSourceHGT1M::getRasterAt(const Coords3D &coords)
{
    int32_t lat = std::floor(coords.lat());
    int32_t lon = std::floor(coords.lon());
    int64_t addr = 0;
    {
        memcpy(&addr, &lat, sizeof(int32_t));
        addr <<= 32;
        memcpy(&addr, &lon, sizeof(int32_t));
    }
    if (m_datasetList.find(addr) != m_datasetList.end())
        return m_datasetList.at(addr);
    char filename[12];
    sprintf(filename, "%s%02d%s%03d.hgt",
            (coords.lat() < 0) ? "S" : "N", std::abs(lat),
            (coords.lon() < 0) ? "W" : "E", std::abs(lon));

    boost::filesystem::path local = m_path.append(std::string(filename));
    GDALDataset *ds = (GDALDataset *)GDALOpen(local.c_str(), GA_ReadOnly);
    RasterObject *ro = nullptr;
    if (ds == NULL)
        ro = new RasterObject;
    else
        ro = new GDALRasterObject(ds);
    m_datasetList.insert(std::pair(addr, ro));
    return ro;
}

HeightSourceHGT1M::GDALRasterObject::GDALRasterObject(GDALDataset *ds) : RasterObject(),
    m_ds(ds)
{
    if (m_ds->GetRasterCount() > 0)
    {
        m_band = m_ds->GetRasterBand(1);
        if (GDALGetGeoTransform(ds, m_transform) != CE_None)
        {
            delete m_band;
            m_band = nullptr;
            delete m_ds;
            m_ds = nullptr;
        }
    }
}

bool HeightSourceHGT1M::GDALRasterObject::isValid()
{
    return m_ds != nullptr;
}

bool HeightSourceHGT1M::GDALRasterObject::valueAtPosition(const Coords3D &pos, int32_t &v)
{
    if (!m_ds || !m_band)
        return false;
    int x = static_cast<int>(std::fabs(std::fabs(pos.lon() - m_transform[0]) / m_transform[1]));
    int y = static_cast<int>(std::fabs(std::fabs(pos.lat() - m_transform[3]) / m_transform[5]));

    int16_t pafScanline = 0;
    CPLErr err = m_band->RasterIO(GF_Read, x, y, 1, 1, &pafScanline, 1, 1, GDT_CInt16, 0, 0);
    if (err == CE_None && pafScanline > -32768)
    {
        v = pafScanline;
        return true;
    }
    return false;
}

}
