#pragma once

#include "../singleton.hpp"

#include "../valve_sdk/csgostructs.hpp"

class Chams
    : public Singleton<Chams>
{
    friend class Singleton<Chams>;

    Chams();
    ~Chams();

public:
    void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix);
private:
    void OverrideMaterial(bool ignorez, int type, const Color& rgba);

    IMaterial* materialRegular = nullptr;
    IMaterial* materialFlat = nullptr;
    IMaterial* materialGlass = nullptr;
    IMaterial* materialGlow = nullptr;
    IMaterial* materialOverlay = nullptr;
    IMaterial* materialAnimated = nullptr;
};
