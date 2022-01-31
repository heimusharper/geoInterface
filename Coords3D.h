#ifndef COORDS3D_H
#define COORDS3D_H
#include <cmath>
#include <geodesic.h>

#if defined(__cpp_concepts)
#include <concepts>
#endif

#include <numeric>
namespace geo
{
template <class T>
#if defined(__cpp_concepts)
requires std::floating_point<T>
#endif

class Coords
{
public:
    Coords() : m_lat(NAN), m_lon(NAN), m_alt(NAN)
    {
    }
    Coords(T lat, T lon, T alt = NAN) : m_lat(lat), m_lon(lon), m_alt(alt)
    {
    }

    void set(T lat, T lon, T alt = 0)
    {
        m_lat = lat;
        m_lon = lon;
        m_alt = alt;
    }

    T lat() const
    {
        return m_lat;
    }
    T lon() const
    {
        return m_lon;
    }
    T alt() const
    {
        return m_alt;
    }

    void setAlt(T a)
    {
        m_alt = a;
    }

    bool operator==(const Coords<T> &coord) const
    {
        return std::abs(coord.lat() - m_lat) < 0.000001 && std::abs(coord.lon() - m_lon) < 0.000001 &&
        std::abs(coord.alt() - m_alt) < 0.01;
    }
    bool operator !=(const Coords<T> &coord) const
    {
        return std::abs(coord.lat() - m_lat) > 0.000001 || std::abs(coord.lon() - m_lon) > 0.000001 ||
        std::abs(coord.alt() - m_alt) > 0.01;
    }

    bool valid() const
    {
        return (!std::isnan(m_alt) || !std::isnan(m_lat) || !std::isnan(m_lon));
    }

    double distanceTo(const Coords<T> &pos)
    {
        double dst;
        double azi1;
        double azi2;
        geod_inverse(geod().geod, this->lat(), this->lon(), pos.lat(), pos.lon(), &dst, &azi1, &azi2);
        return std::fabs(dst);
    }

    Coords<T> pointTo(double distance, double angle)
    {
        double lat;
        double lon;
        double azi2;
        geod_direct(geod().geod, this->lat(), this->lon(), angle, distance, &lat, &lon, &azi2);
        return Coords<T>((T)lat, (T)lon, 0);

    }

private:
    T m_lat;
    T m_lon;
    T m_alt;

    struct Geod {
        Geod()
        {
            geod           = new geod_geodesic;
            const double a = 6378137;
            const double f = 1 / 298.257223563; /* WGS84 */
            geod_init(geod, a, f);
        }
        ~Geod()
        {
            delete geod;
        }
        geod_geodesic *geod = nullptr;
    };

    static Geod &geod()
    {
        static Geod g;
        return g;
    }
};

typedef Coords<double> Coords3D;
typedef Coords<float> Coords3F;

}

#endif // COORDS3D_H
