#include "material.h"
#include "vector3f.h"
#include <iostream>

namespace fst
{
    Material::Material(const math::Vector3f& ambient, const math::Vector3f& diffuse, const math::Vector3f& specular, const math::Vector3f& mirror, float phong_exponent)
        : m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_mirror(mirror)
        , m_phong_exponent(phong_exponent)
    {}

    math::Vector3f Material::computeBrdf(const math::Vector3f& wi, const math::Vector3f& wo, const math::Vector3f& normal, const math::Vector3f& textConstant, int statusCode) const
    {   
        //Normal
        if(statusCode == 0){
            auto diffuse = math::max(math::dot(normal, wi), 0.0f);
            auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);

            return m_specular * specular + m_diffuse * diffuse;
        }
        //Replace kd
        else if(statusCode == 1){
            auto diffuse = math::max(math::dot(normal, wi), 0.0f);
            auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);

            return m_specular * specular + (textConstant/255) * diffuse;
        }
        //Blend kd
        else if(statusCode == 2){
            auto diffuse = math::max(math::dot(normal, wi), 0.0f);
            auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);

            return m_specular * specular + ((m_diffuse + (textConstant/255))/2) * diffuse;
        }
        //Replace all
        else{

            auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);
            return m_specular * specular;
        }
    }
}