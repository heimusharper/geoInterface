#ifndef HEIGHTSOURCEHGT1M_H
#define HEIGHTSOURCEHGT1M_H
#include "HeightSourceInterface.h"
#include <gdal/gdal_priv.h>
#include <gdal/cpl_conv.h>
#include <filesystem>
#include <list>

namespace geo {
class HeightSourceHGT1M : public HeightSourceInterface
{
public:
    HeightSourceHGT1M(const std::filesystem::path &path);
    virtual bool heightAt(const Coords3D &coord, const int32_t nd, int32_t &out) override;
    virtual void cacheArea(const Coords3D &leftTop, const Coords3D &rightBottom) override;

private:

    std::filesystem::path m_path;

private:

    class RasterObject
    {
    public:
        RasterObject() {}
        virtual ~RasterObject() = default;
        virtual bool isValid()
        {
            return false;
        }
        virtual bool valueAtPosition(const Coords3D &pos, int32_t &v)
        {
            return false;
        }
    };
    class GDALRasterObject : public RasterObject
    {
    public:
        GDALRasterObject(GDALDataset *ds);
        virtual bool isValid() override;
        virtual bool valueAtPosition(const Coords3D &pos, int32_t &v) override;
    private:
        GDALDataset *m_ds = nullptr;
        GDALRasterBand  *m_band = nullptr;
        double m_transform[6] = {};
    };

    std::map<int64_t, RasterObject*> m_datasetList;

    RasterObject *getRasterAt(const Coords3D &coords);

};
}

#endif // HEIGHTSOURCEHGT1M_H
