#pragma once

/// <summary>
/// �����蔻�葮��
/// </summary>

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b1 << 0;//�n�`
const unsigned short COLLISION_ATTR_PLANE_CURSOR = 0b1 << 1;//�J�[�\��
const unsigned short COLLISION_ATTR_OBJECT_SPHERE = (0b1 << 2) + 0b00;//�X�e�[�W�I�u�W�F�N�g(��)
const unsigned short COLLISION_ATTR_OBJECT_MESH = (0b1 << 2) + 0b01;//�X�e�[�W�I�u�W�F�N�g(���b�V��)
const unsigned short COLLISION_ATTR_OBJECT_NONE = (0b1 << 2) + 0b10;//�X�e�[�W�I�u�W�F�N�g(�g���Ă��Ȃ����p)
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 3;//����
const unsigned short COLLISION_ATTR_ENEMIES = 0b1 << 4;//�G
const unsigned short COLLISION_ATTR_WEAPONS = 0b1 << 5;//����