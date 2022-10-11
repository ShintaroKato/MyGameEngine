#pragma once

/// <summary>
/// 当たり判定属性
/// </summary>

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b1 << 0;//地形
const unsigned short COLLISION_ATTR_OBJECT = 0b1 << 1;//ステージオブジェクト
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 2;//味方
const unsigned short COLLISION_ATTR_ENEMIES = 0b1 << 3;//敵
const unsigned short COLLISION_ATTR_WEAPONS = 0b1 << 4;//武器