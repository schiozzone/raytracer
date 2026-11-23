#include "geometry/box.hpp"

#include "geometry/aarect.hpp"

Box::Box(const Vec3& p0, const Vec3& p1, std::shared_ptr<Material> mat)
    : box_min_(p0), box_max_(p1) {
	sides_.add(std::make_shared<XyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));
	sides_.add(std::make_shared<XyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
	sides_.add(std::make_shared<XzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));
	sides_.add(std::make_shared<XzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
	sides_.add(std::make_shared<YzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
	sides_.add(std::make_shared<YzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
}
