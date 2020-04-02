//
// Created by dominik on 20.06.19.
//

#include "Material.h"


Material::Material()
= default;

Material::~Material()
= default;

void MaterialPBR::bind()
{
    albedo->bind(0);
    ambient->bind(1);
    metalness->bind(2);
    roughness->bind(3);
    normal->bind(4);

}

MaterialPBR::MaterialPBR(const std::shared_ptr<Texture2D> &albedo,
                         const std::shared_ptr<Texture2D> &ambient,
                         const std::shared_ptr<Texture2D> &metalness,
                         const std::shared_ptr<Texture2D> &roughness,
                         const std::shared_ptr<Texture2D> &normal
                         ) :
                         albedo(albedo), ambient(ambient), metalness(metalness),  roughness(roughness), normal(normal)
{
    type = Type::PBR;
}


MaterialPBR::~MaterialPBR()
= default;

void MaterialPHONG::bind()
{

}

MaterialPHONG::MaterialPHONG(const std::shared_ptr<Texture2D> &color,
                             const std::shared_ptr<Texture2D> &normal
                             ) : color(color), normal(normal)
{
    type = Type::PHONG;
}


MaterialPHONG::~MaterialPHONG()
= default;

void MaterialFUR::bind()
{
}

MaterialFUR::MaterialFUR(const std::shared_ptr<Texture2D> &color,
                         const std::shared_ptr<Texture2D> &normal,
                         const std::shared_ptr<Texture2D> &height
                         ) : color(color), normal(normal), height(height)
{
    type = Type::FUR;
}


MaterialFUR::~MaterialFUR()
= default;

void MaterialCUSTOM::bind()
{

}

MaterialCUSTOM::MaterialCUSTOM(const std::vector<std::pair<std::shared_ptr<Texture2D>, unsigned int>> &textures
                                ) : textures(textures)
{
}


MaterialCUSTOM::~MaterialCUSTOM()
= default;

Material::Type Material::getType() const
{
    return type;
}
