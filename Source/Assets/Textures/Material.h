//
// Created by dominik on 20.06.19.
//

#ifndef GAMEENGINE_MATERIAL_H
#define GAMEENGINE_MATERIAL_H

#include <memory>
#include <vector>

#include "Texture.h"

class Material{
public:
    enum class Type{
        PBR,
        PHONG,
        FUR,
        CUSTOM
    };

    virtual ~Material();
    virtual void bind() = 0;

protected:
    Material();
};


class MaterialPBR : public Material{
public:
    MaterialPBR(const std::shared_ptr<Texture2D> &albedo    ,
                const std::shared_ptr<Texture2D> &ambient   ,
                const std::shared_ptr<Texture2D> &metalness ,
                const std::shared_ptr<Texture2D> &roughness ,
                const std::shared_ptr<Texture2D> &normal    );

    ~MaterialPBR() override;

    void bind() override;

private:
    std::shared_ptr<Texture2D> albedo;
    std::shared_ptr<Texture2D> ambient;
    std::shared_ptr<Texture2D> metalness;
    std::shared_ptr<Texture2D> roughness;
    std::shared_ptr<Texture2D> normal;
};


class MaterialPHONG : public Material{
public:
    MaterialPHONG(const std::shared_ptr<Texture2D> &color   ,
                  const std::shared_ptr<Texture2D> &normal  );

    ~MaterialPHONG() override;
    void bind() override;

private:
    std::shared_ptr<Texture2D> color;
    std::shared_ptr<Texture2D> normal;
};


class MaterialFUR : public Material{
public:
    MaterialFUR(const std::shared_ptr<Texture2D> &color  ,
                const std::shared_ptr<Texture2D> &normal ,
                const std::shared_ptr<Texture2D> &height );

    ~MaterialFUR() override;
    void bind() override;

private:
    std::shared_ptr<Texture2D> color;
    std::shared_ptr<Texture2D> normal;
    std::shared_ptr<Texture2D> height;
};


class MaterialCUSTOM : public Material{
public:
    MaterialCUSTOM(const std::vector<std::pair<std::shared_ptr<Texture2D>, unsigned int>> &textures);

    ~MaterialCUSTOM() override;
    void bind() override;

private:
    std::vector<std::pair<std::shared_ptr<Texture2D>, unsigned int>> textures;
};


#endif //GAMEENGINE_MATERIAL_H
