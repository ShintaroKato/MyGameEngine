#pragma once

/// <summary>
/// 当たり判定属性
/// </summary>

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b1 << 0;//地形
const unsigned short COLLISION_ATTR_PLANE_CURSOR = 0b1 << 1;//カーソル
const unsigned short COLLISION_ATTR_OBJECT_SPHERE = (0b1 << 2) + 0b00;//ステージオブジェクト(球)
const unsigned short COLLISION_ATTR_OBJECT_MESH = (0b1 << 2) + 0b01;//ステージオブジェクト(メッシュ)
const unsigned short COLLISION_ATTR_OBJECT_NONE = (0b1 << 2) + 0b10;//ステージオブジェクト(使われていない時用)
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 3;//味方
const unsigned short COLLISION_ATTR_ENEMIES = 0b1 << 4;//敵
const unsigned short COLLISION_ATTR_WEAPONS = 0b1 << 5;//武器