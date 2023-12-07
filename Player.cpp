#include "Player.h"

// ���ӿ��� ����ϰ� ����ϴ� ��ǥ���� ��ȯ �۾����� ���� �ִ���?
// ũ�� (���� ��ȯ), ȸ�� (���� ��ȯ), �̵� (���� ��ȯ�� �ƴ�)
// ���� ��ȯ - ������ �ٲ��� �ʴ� ��ȯ
// ���� ��ȯ�� �����ϴٸ� ��ķ� ��ȯ�� ����������, ��ķ� ��ȯ�� ���������ٸ� ������ ��ȯ�� �� ��Ŀ� �����Ͽ� ��ȯ�� �����ϴ�.
// ���� �ӵ��� �޸� ���� ���鿡�� �̵��� ì��

void Player::Initialize()
{
	speed = 2.f;
	moveDisplacement = D3DXVECTOR3(0, 0, 0);
	info.pos = D3DXVECTOR3(WIN_SIZE_X / 2.f, WIN_SIZE_Y / 2.f, 0);
	info.dir = D3DXVECTOR3(0, 0, 0);
	info.look = D3DXVECTOR3(1.f, 0, 0);

	bodyAngle = 0;
	gunBarrelAngle = 0;
	scale = 50.f;
	afterScale = scale;
	scaleRatio = 1.f;

	// ��, ����
	originPoint[0].x = info.pos.x - scale;
	originPoint[0].y = info.pos.y - scale;

	// ��, ����
	originPoint[1].x = info.pos.x + scale;
	originPoint[1].y = info.pos.y - scale;

	// ��, �Ʒ���
	originPoint[2].x = info.pos.x + scale;
	originPoint[2].y = info.pos.y + scale;

	// ��, �Ʒ���
	originPoint[3].x = info.pos.x - scale;
	originPoint[3].y = info.pos.y + scale;

	gunBarrelDistance = 100.f;
}

void Player::Update()
{
	if (GetAsyncKeyState('A'))
	{
		bodyAngle -= D3DXToRadian(5.f);
	}

	if (GetAsyncKeyState('D'))
	{
		bodyAngle += D3DXToRadian(5.f);
	}

	if (GetAsyncKeyState('W'))
	{
		D3DXVECTOR3 temp;
		temp.x = info.look.x * cosf(bodyAngle) - info.look.y * sinf(bodyAngle);
		temp.y = info.look.x * sinf(bodyAngle) + info.look.y * cosf(bodyAngle);
		temp.z = 0;

		info.pos += temp;
	}

	if (GetAsyncKeyState('Q'))
	{
		afterScale -= 1.f;
	}

	if (GetAsyncKeyState('E'))
	{
		afterScale += 1.f;
	}

	scaleRatio = afterScale / scale;

	// ũ�� ���
	D3DXMatrixScaling(&matScale, scaleRatio, scaleRatio, 1.f);

	// ȸ�� ��� (�Ķ���ͷ� �ѱ� ������ ����)
	D3DXMatrixRotationZ(&matRotZ, bodyAngle);

	// ũ�� * ȸ�� ����� ����� ����
	matSR = matScale * matRotZ;

	// �̵� ���
	D3DXMatrixTranslation(&matTrans, info.pos.x, info.pos.y, info.pos.z);

	// ��� ������ ����
	// ũ�� * ����(�ڱ� �ڽ� ȸ��) * �̵� * ���� * (�������� �θ� �����Ѵٸ� ��ü�� ���� ��� -> ũ���̰���)�� ���� ����� ���� ����� �ȴ�.
	matWorld = matScale * matRotZ * matTrans;

#pragma region ����
	// Ŭ���̾�Ʈ ���콺 ��ǥ �ޱ�
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);

	// ����Ʈ ����ü�� �� ���콺 ��ǥ�� ���͵���� ������
	// ���ϰ� �ϱ� ����, ���콺 ��ǥ Ÿ���� ���ͷ� ��ȯ (����ü ������ ���)
	D3DXVECTOR3 mousePos(mouse.x, mouse.y, 0);

	// �÷��̾ ���콺�� �ٶ󺸴� ���� ���͸� ���Ѵ�.
	info.dir = mousePos - info.pos;
	// �� ����(x, y, z)�� ������ ũ��� ������ ��
	//float length = sqrt(info.dir.x * info.dir.x + info.dir.y * info.dir.y);
	//info.dir.x /= length;
	//info.dir.y /= length;
	// ���� ������� �ۼ����� �ʰ� �Լ��� �������͸� ����� ������ �غ��ٸ�
	D3DXVec3Normalize(&info.dir, &info.dir);

	// look ���ʹ� ���� ���� (���� 2d x��)���� ���� ���� ũ�Ⱑ 1�� ����
	// �ش� ������ ���а� ĳ���Ͱ� �ٶ󺸴� ���� ���г��� ���� ��,
	// ������� ������ ��, ��������� x�� ���� ����
	// �� ��, dir�� ũ�Ⱑ 1�� ���� �����̹Ƿ� ���� ���� �����Ѵٰ� �� �� ����
	// ���� ���� �����Ѵٸ�, x�� ���� cos ��Ÿ�� �ǹ���
	// cos ��Ÿ�� �غ� / �����̰�, �ش� ���� ���Լ��� ���� ��, ��������� ���ϴ� ��Ÿ ���� ���� �� ����
	//gunBarrelAngle = acosf(info.dir.x * info.look.x + info.dir.y * info.look.y);
	// ������ ���� �Լ��� ����
	gunBarrelAngle = acosf(D3DXVec3Dot(&info.dir, &info.look));

	// ��ũ �ڻ����� 180������ �ۿ� ǥ���� ���ϴ� ���� ó��
	if (mousePos.y > info.pos.y)
	{
		gunBarrelAngle = 2 * D3DX_PI - gunBarrelAngle;
	}

	// ���� ��ġ ����
	gunBarrelPosX = cosf(gunBarrelAngle) * gunBarrelDistance + info.pos.x + moveDisplacement.x;
	gunBarrelPosY = -sinf(gunBarrelAngle) * gunBarrelDistance + info.pos.y + moveDisplacement.y;
#pragma endregion
	// info.pos�� ���� �̵��ϹǷ�,
	// pos�� �Է¿� ���� ����ǹǷ�, ���� ������ �Ź� �������ش�.
	// ��, ����
	originPoint[0].x = info.pos.x - afterScale;
	originPoint[0].y = info.pos.y - afterScale;

	// ��, ����
	originPoint[1].x = info.pos.x + afterScale;
	originPoint[1].y = info.pos.y - afterScale;

	// ��, �Ʒ���
	originPoint[2].x = info.pos.x + afterScale;
	originPoint[2].y = info.pos.y + afterScale;

	// ��, �Ʒ���
	originPoint[3].x = info.pos.x - afterScale;
	originPoint[3].y = info.pos.y + afterScale;

	// ����Ʈ ����ü ���� ��� ��Ƶ� ����
	//POINT temp;
	// ĳ������ �� ���� ���� �����̴� ����
	for (int i = 0; i < 4; ++i)
	{
		// ���� ���� ��������
		calPoint[i] = originPoint[i];

		// �÷��̾��� ��ġ�� ������ ���� ��ǥ�� ���ϱ�
		calPoint[i].x -= info.pos.x;
		calPoint[i].y -= info.pos.y;

		// ���� ��ǥ ����� ���� �� ����Ʈ���� ���
		calPointBack[i] = calPoint[i];

		// ���� ��ġ�� ���� ����� ���� ����� ����
		// ����(��Ȯ���� ��) * ��� ����
		D3DXVec3TransformCoord(&calPoint[i], &calPoint[i], &matWorld); // ���� ���궧 ����ϴ� �Լ�
		//D3DXVec3TransformNormal(); // ���Ϳ� ���궧 ����ϴ� �Լ�

		//// ���� ��ǥ�� �̿��� ȸ��
		//temp = calPoint[i];
		//// �ﰢ�Լ��� ��������
		//// cosf(x�� ȸ���� ��ǥ) = cosf * cosf - sinf * sinf
		//// sinf(y�� ȸ���� ��ǥ) = cosf * sinf + sinf * cosf
		//// -> ������ �İ� ������ ����
		//calPoint[i].x = temp.x * cosf(bodyAngle) + (-temp.y * sinf(bodyAngle));
		//calPoint[i].y = temp.x * sinf(bodyAngle) + temp.y * cosf(bodyAngle);

		// ���� ȸ���� �������Ƿ�, �ٽ� �÷��̾��� ��ġ�� ���� ���������� ȸ���� ��ġ�� ��Ÿ��
		//���� ��ǥ�� ���� ��ǥ�� �ٲ�
		//calPoint[i].x += info.pos.x;
		//calPoint[i].y += info.pos.y;
	}
}

void Player::Render(HDC hdc)
{
	// ȭ�� ����� �뵵
	Rectangle(hdc, 0, 0, WIN_SIZE_X, WIN_SIZE_Y);

	PrintInfo(hdc);

	// �÷��̾� �׸���
	// ������
	MoveToEx(hdc, calPoint[0].x, calPoint[0].y, NULL);
	// �� �׸���
	LineTo(hdc, calPoint[1].x, calPoint[1].y);
	LineTo(hdc, calPoint[2].x, calPoint[2].y);
	LineTo(hdc, calPoint[3].x, calPoint[3].y);
	LineTo(hdc, calPoint[0].x, calPoint[0].y);

	// ���� �׸���
	MoveToEx(hdc, info.pos.x, info.pos.y, NULL);
	LineTo(hdc, gunBarrelPosX, gunBarrelPosY);
}

void Player::Release()
{
}

void Player::PrintInfo(HDC hdc)
{
	// ���콺 ��ǥ
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);
	PrintVector(hdc, L"���콺", D3DXVECTOR2(mouse.x, mouse.y), D3DXVECTOR2(10, 10));

	// ���⺤�� (�÷��̾ ���콺���� �ٶ󺸴� ����) (�������α׷��� ��ǥ�� �Ʒ��� ������ ���� ��ǥ)
	PrintVector(hdc, L"���� ����", D3DXVECTOR2(info.dir.x, info.dir.y), D3DXVECTOR2(10, 30));

	// ��ü�� ȸ�� ���� (�÷��̾ 0������ �󸶳� ȸ���ߴ���)
	PrintVector(hdc, L"��ü ȸ�� ����", D3DXVECTOR2(bodyAngle, D3DXToDegree(bodyAngle)), D3DXVECTOR2(10, 50));

	// ������ ���� (����)
	PrintVector(hdc, L"���Ű� (r, d)", D3DXVECTOR2(gunBarrelAngle, D3DXToDegree(gunBarrelAngle)), D3DXVECTOR2(10, 70));

	// ���� ��ġ
	PrintVector(hdc, L"���� ��ġ", D3DXVECTOR2(gunBarrelPosX, gunBarrelPosY), D3DXVECTOR2(10, 90));

	// ���� ��� ���� ��, �� ���� ��ġ
	for (int i = 0; i < 4; ++i)
	{
		wchar_t str[32];
		swprintf_s(str, L"��ȯ �� �� %d", i);
		PrintVector(hdc, str, D3DXVECTOR2(calPointBack[i].x, calPointBack[i].y), D3DXVECTOR2(10, 120 + i * 20));
	}

	// ���� ��� ���� ��, �� ���� ��ġ
	for (int i = 0; i < 4; ++i)
	{
		wchar_t str[32];
		swprintf_s(str, L"��ȯ �� �� %d", i);
		PrintVector(hdc, str, D3DXVECTOR2(calPoint[i].x, calPoint[i].y), D3DXVECTOR2(10, 220 + i * 20));
	}

	// ��� ���� ���
	PrintMat(hdc, L"����", matWorld, D3DXVECTOR2(WIN_SIZE_X - 300, 30));
	PrintMat(hdc, L"ũ��", matScale, D3DXVECTOR2(WIN_SIZE_X - 300, 150));
	PrintMat(hdc, L"ȸ��(z)", matRotZ, D3DXVECTOR2(WIN_SIZE_X - 300, 300));
	PrintMat(hdc, L"�̵�", matTrans, D3DXVECTOR2(WIN_SIZE_X - 300, 450));
	PrintMat(hdc, L"ũ�� * ȸ��", matSR, D3DXVECTOR2(WIN_SIZE_X - 600, 450));
}

void Player::PrintVector(HDC hdc, const wchar_t* text, D3DXVECTOR2 vec2, D3DXVECTOR2 textPt)
{
	wchar_t temp[128];

	swprintf_s(temp, L"%s : %.2f, %.2f", text, vec2.x, vec2.y);

	TextOut(hdc, textPt.x, textPt.y, temp, wcslen(temp));
}

void Player::PrintMat(HDC hdc, const wchar_t* text, D3DXMATRIX mat, D3DXVECTOR2 textPt)
{
	wchar_t temp[128];

	swprintf_s(temp, L"%s", text);
	TextOut(hdc, textPt.x, textPt.y - 20, temp, wcslen(temp));

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			swprintf_s(temp, L"%.2f", mat.m[i][j]);
			TextOut(hdc, textPt.x + 60 * j, textPt.y + 20 * i, temp, wcslen(temp));
		}
	}
}

Player::Player()
{
}

Player::~Player()
{
}
