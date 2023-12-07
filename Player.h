#pragma once
#include "Object.h"

class Player :
    public Object
{
private:
    D3DXVECTOR3 originPoint[4]; // 원점(4개의 꼭짓점)의 정보
    D3DXVECTOR3 calPointBack[4]; // 행렬 연산이 적용되지 않은 점들
    D3DXVECTOR3 calPoint[4]; // 원점의 정보를 가지고 연산한 점들의 정보를 저장하는 배열

    int speed;
    // 탱크 몸체 회전 시, 사용할 각
    float bodyAngle;
    // 포신의 길이
    float gunBarrelDistance;
    // 포신 회전 시, 사용할 각
    float gunBarrelAngle;
    // 포신 위치
    float gunBarrelPosX;
    float gunBarrelPosY;
    // 키 입력으로 입력받은 이동량을 가질 벡터
    D3DXVECTOR3 moveDisplacement;

    D3DXMATRIX matScale; // 크기 행렬
    D3DXMATRIX matRotZ; // 회전 행렬 (z축 회전)
    D3DXMATRIX matTrans; // 이동 행렬

    D3DXMATRIX matSR; // 크기 * 회전을 담을 행렬
    D3DXMATRIX matWorld; // 월드 행렬 (크기 * 회전 * 이동)
    float scale; // 원래 스케일
    float afterScale; // 키입력을 통해 변경된 스케일
    float scaleRatio; // 원래 스케일과 바뀐 스케일의 비율

public:
    // Object을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render(HDC hdc) override;
    void Release() override;

public:
    void PrintInfo(HDC hdc);
    void PrintVector(HDC hdc, const wchar_t* text, D3DXVECTOR2 vec2, D3DXVECTOR2 textPt);
    void PrintMat(HDC hdc, const wchar_t* text, D3DXMATRIX mat, D3DXVECTOR2 textPt);

public:
    Player();
    virtual ~Player();
};

