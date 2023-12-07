#pragma once
#include "Object.h"

class Player :
    public Object
{
private:
    D3DXVECTOR3 originPoint[4]; // ����(4���� ������)�� ����
    D3DXVECTOR3 calPointBack[4]; // ��� ������ ������� ���� ����
    D3DXVECTOR3 calPoint[4]; // ������ ������ ������ ������ ������ ������ �����ϴ� �迭

    int speed;
    // ��ũ ��ü ȸ�� ��, ����� ��
    float bodyAngle;
    // ������ ����
    float gunBarrelDistance;
    // ���� ȸ�� ��, ����� ��
    float gunBarrelAngle;
    // ���� ��ġ
    float gunBarrelPosX;
    float gunBarrelPosY;
    // Ű �Է����� �Է¹��� �̵����� ���� ����
    D3DXVECTOR3 moveDisplacement;

    D3DXMATRIX matScale; // ũ�� ���
    D3DXMATRIX matRotZ; // ȸ�� ��� (z�� ȸ��)
    D3DXMATRIX matTrans; // �̵� ���

    D3DXMATRIX matSR; // ũ�� * ȸ���� ���� ���
    D3DXMATRIX matWorld; // ���� ��� (ũ�� * ȸ�� * �̵�)
    float scale; // ���� ������
    float afterScale; // Ű�Է��� ���� ����� ������
    float scaleRatio; // ���� �����ϰ� �ٲ� �������� ����

public:
    // Object��(��) ���� ��ӵ�
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

