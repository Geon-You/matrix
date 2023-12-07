#pragma once
#include "framework.h"

#define WIN_SIZE_X 800
#define WIN_SIZE_Y 600

extern HWND g_hWnd;

// 특정 객체를 그릴 때, 위치와 사이즈가 필요
// 그럼 그 때 마다 위치와 사이즈를 매번 새로 선언하기 귀찮으니까
// 자주 사용되는 데이터를 구조체로 만들어 처리
typedef struct tagInfo
{
	D3DXVECTOR3 pos; // 위치
	D3DXVECTOR3 dir; // 방향
	// 0도를 바라보는 크기가 1인 단위 벡터 (1, 0, 0)
	// -> 각종 연산을 편리하게 하기 위해서
	D3DXVECTOR3 look;
}INFO;

