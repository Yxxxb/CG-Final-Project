import { Vector3 } from "./vec3.js";
import { Matrix4 } from "./mat4.js";

// 摄像机类
export class Player
{
    constructor(keys, mouse)
    {
        this.keys = keys;
        this.mouse = mouse;

        this.speed = 3.0;
        this.rotSpeed = 60.0;

        this.pos = new Vector3(0.0, 0.0, 0.0);
        this.rot = new Vector3(0.0, 0.0, 0.0);
        this.cameraTransform = new Matrix4();
    }

    update(delta)
    {
        this.speed = 3.0;
        //加速键
        if (this.keys.shift) this.speed = 6.0;
        //位置右移值和下移值
        let ax = 0.0;
        let az = 0.0;
        //用户坐标系移动判断，全都是1，用来标识单位方向
        if (this.keys.left) ax--;
        if (this.keys.right) ax++;
        if (this.keys.up) az--;
        if (this.keys.down) az++;

        // this.pos.x += (Math.cos(this.rot.y * Math.PI / 180.0) * ax + Math.sin(this.rot.y * Math.PI / 180.0) * az) * this.speed * delta;
        // this.pos.z += (-Math.sin(this.rot.y * Math.PI / 180.0) * ax + Math.cos(this.rot.y * Math.PI / 180.0) * az) * this.speed * delta;
        //上下移动的标识，左右旋转的标识
        if (this.keys.space) this.pos.y += this.speed * delta;
        if (this.keys.c) this.pos.y -= this.speed * delta;
        if (this.keys.q) this.rot.y += this.rotSpeed * delta;
        if (this.keys.e) this.rot.y -= this.rotSpeed * delta;

        //rot.y是水平的旋转 rot.x是竖直的旋转
        if (this.mouse.down)
        {
            this.rot.y -= this.mouse.dx * 0.1 * this.rotSpeed * delta;
            this.rot.x -= this.mouse.dy * 0.1 * this.rotSpeed * delta;
        }
        // console.log(ax,az)
        //通过用户坐标系移动得到世界坐标系的位置变换
        this.pos.x += (Math.cos(this.rot.y * Math.PI / 180.0) * ax + Math.sin(this.rot.y * Math.PI / 180.0) * az) * this.speed * delta;
        this.pos.z += (-Math.sin(this.rot.y * Math.PI / 180.0) * ax + Math.cos(this.rot.y * Math.PI / 180.0) * az) * this.speed * delta;
        //rot.z一直为0
        const radRot = this.rot.mul(-Math.PI / 180.0);
        this.cameraTransform = new Matrix4().rotate(radRot.x, radRot.y, radRot.z);
        //得到最终的变换矩阵
        this.cameraTransform = this.cameraTransform.translate(-this.pos.x, -this.pos.y, -this.pos.z);
    }
}
