https://computergraphics.stackexchange.com/questions/2316/is-russian-roulette-really-the-answer
In order to understand Russian Roulette, let's look at a very basic backward path tracer:

```cpp
void RenderPixel(uint x, uint y, UniformSampler *sampler) {
    Ray ray = m_scene->Camera.CalculateRayFromPixel(x, y, sampler);

    float3 color(0.0f);
    float3 throughput(1.0f);

    // Bounce the ray around the scene
    for (uint bounces = 0; bounces < 10; ++bounces) {
        m_scene->Intersect(ray);

        // The ray missed. Return the background color
        if (ray.geomID == RTC_INVALID_GEOMETRY_ID) {
            color += throughput * float3(0.846f, 0.933f, 0.949f);
            break;
        }

        // We hit an object

        // Fetch the material
        Material *material = m_scene->GetMaterial(ray.geomID);
        // The object might be emissive. If so, it will have a corresponding light
        // Otherwise, GetLight will return nullptr
        Light *light = m_scene->GetLight(ray.geomID);

        // If we hit a light, add the emmisive light
        if (light != nullptr) {
            color += throughput * light->Le();
        }

        float3 normal = normalize(ray.Ng);
        float3 wo = normalize(-ray.dir);
        float3 surfacePos = ray.org + ray.dir * ray.tfar;

        // Get the new ray direction
        // Choose the direction based on the material
        float3 wi = material->Sample(wo, normal, sampler);
        float pdf = material->Pdf(wi, normal);

        // Accumulate the brdf attenuation
        throughput = throughput * material->Eval(wi, wo, normal) / pdf;


        // Shoot a new ray

        // Set the origin at the intersection point
        ray.org = surfacePos;

        // Reset the other ray properties
        ray.dir = wi;
        ray.tnear = 0.001f;
        ray.tfar = embree::inf;
        ray.geomID = RTC_INVALID_GEOMETRY_ID;
        ray.primID = RTC_INVALID_GEOMETRY_ID;
        ray.instID = RTC_INVALID_GEOMETRY_ID;
        ray.mask = 0xFFFFFFFF;
        ray.time = 0.0f;
    }

    m_scene->Camera.FrameBuffer.SplatPixel(x, y, color);
}
```
IE. we bounce around the scene, accumulating color and light attenuation as we go. In
order to be completely mathematically unbiased, bounces should go to infinity. But this
is unrealistic, and as you noted, not visually necessary; for most scenes, after a
certain number of bounces, say 10, the amount of contribution to the final color is very
very minimal.

So in order to save computing resources, many path tracers have a hard limit to the
number of bounces. This adds bias.

That said, it's hard to choose what that hard limit should be. Some scenes look great
after 2 bounces; others (say with transmission or SSS) may take up to 10 or 20. 

If we choose too low, the image will be visibly biased. But if we choose too high, we're
wasting computation energy and time.

One way to solve this, as you noted, is to terminate the path after we reach some
threshold of attenuation. This also adds bias.

Clamping after a threshold, will work, but again, how do we choose the threshold? If we
choose too large, the image will be visibly biased, too small, and we're wasting
resources.

Russian Roulette attempts to solve these problems in an unbiased way. First, here is the
code:

```cpp
void RenderPixel(uint x, uint y, UniformSampler *sampler) {
    Ray ray = m_scene->Camera.CalculateRayFromPixel(x, y, sampler);

    float3 color(0.0f);
    float3 throughput(1.0f);

    // Bounce the ray around the scene
    for (uint bounces = 0; bounces < 10; ++bounces) {
        m_scene->Intersect(ray);

        // The ray missed. Return the background color
        if (ray.geomID == RTC_INVALID_GEOMETRY_ID) {
            color += throughput * float3(0.846f, 0.933f, 0.949f);
            break;
        }

        // We hit an object

        // Fetch the material
        Material *material = m_scene->GetMaterial(ray.geomID);
        // The object might be emissive. If so, it will have a corresponding light
        // Otherwise, GetLight will return nullptr
        Light *light = m_scene->GetLight(ray.geomID);

        // If we hit a light, add the emmisive light
        if (light != nullptr) {
            color += throughput * light->Le();
        }

        float3 normal = normalize(ray.Ng);
        float3 wo = normalize(-ray.dir);
        float3 surfacePos = ray.org + ray.dir * ray.tfar;

        // Get the new ray direction
        // Choose the direction based on the material
        float3 wi = material->Sample(wo, normal, sampler);
        float pdf = material->Pdf(wi, normal);

        // Accumulate the brdf attenuation
        throughput = throughput * material->Eval(wi, wo, normal) / pdf;


        // Russian Roulette
        // Randomly terminate a path with a probability inversely equal to the throughput
        float p = std::max(throughput.x, std::max(throughput.y, throughput.z));
        if (sampler->NextFloat() > p) {
            break;
        }

        // Add the energy we 'lose' by randomly terminating paths
        throughput *= 1 / p;


        // Shoot a new ray

        // Set the origin at the intersection point
        ray.org = surfacePos;

        // Reset the other ray properties
        ray.dir = wi;
        ray.tnear = 0.001f;
        ray.tfar = embree::inf;
        ray.geomID = RTC_INVALID_GEOMETRY_ID;
        ray.primID = RTC_INVALID_GEOMETRY_ID;
        ray.instID = RTC_INVALID_GEOMETRY_ID;
        ray.mask = 0xFFFFFFFF;
        ray.time = 0.0f;
    }

    m_scene->Camera.FrameBuffer.SplatPixel(x, y, color);
}
```

Russian Roulette randomly terminates a path with a probability inversely equal to the
throughput. So paths with low throughput that won't contribute much to the scene are more
likely to be terminated.

If we stop there, we're still biased. We 'lose' the energy of the path we randomly
terminate. To make it unbiased, we boost the energy of the non-terminated paths by their
probability to be terminated. This, along with being random, makes Russian Roulette
unbiased.

To answer your last questions:

* Does Russian Roulette give an unbiased result?
    * Yes
* Is Russian Roulette necessary for an unbiased result?
    * Depends on what you mean by unbiased. If you mean mathematically, then yes.
      However, if you mean visually, then no. You just have to choose you max path depth
      and cutoff threshold very very carefully. This can be very tedious since it can
      change from scene to scene.
* Can you use a fixed probability (cut-off), and then redistribute the 'lost' energy. Is
  this unbiased?
    * If you use a fixed probability, you are adding bias. By redistributing the 'lost'
      energy, you reduce the bias, but it is still mathematically biased. To be
      completely unbiased, it must be random.
* If the energy that would be lost by terminating a ray without redistributing its energy
  is eventually lost anyway (as the rays to which it is redistributed are also eventually
  terminated), how does this improve the situation?
    * Russian Roulette only stops the bouncing. It doesn't remove the sample completely.
      Also, the 'lost' energy is accounted for in the bounces up to the termination. So
      the only way for the the energy to be 'eventually lost anyway' would be to have a
      completely black room.

In the end, Russian Roulette is a very simple algorithm that uses a very small amount of
extra computational resources. In exchange, it can save a large amount of computational
resources. Therefore, I can't really see a reason not to use it.
