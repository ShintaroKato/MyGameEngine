#pragma once

/// <summary>
/// �����蔻�葮��
/// </summary>

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b1 << 0;//�n�`
const unsigned short COLLISION_ATTR_OBJECT = 0b1 << 1;//�X�e�[�W�I�u�W�F�N�g
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 2;//����
const unsigned short COLLISION_ATTR_ALLIES_WEAPON = COLLISION_ATTR_ALLIES + 1;//����(����)
const unsigned short COLLISION_ATTR_ENEMIES = 0b1 << 3;//�G
const unsigned short COLLISION_ATTR_ENEMIES_WEAPON = COLLISION_ATTR_ENEMIES + 1;//�G(����)