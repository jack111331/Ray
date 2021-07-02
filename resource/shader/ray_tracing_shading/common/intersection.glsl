float AABBIntersect(vec3 minCorner, vec3 maxCorner, Ray r)
//----------------------------------------------------------------
{
    vec3 invdir = 1.0f / r.velocity;

    vec3 f = (maxCorner - r.origin) * invdir;
    vec3 n = (minCorner - r.origin) * invdir;

    vec3 tmax = max(f, n);
    vec3 tmin = min(f, n);

    float t1 = min(tmax.x, min(tmax.y, tmax.z));
    float t0 = max(tmin.x, max(tmin.y, tmin.z));

    return (t1 >= t0) ? (t0 > 0.0f ? t0 : t1) : -1.0f;
}