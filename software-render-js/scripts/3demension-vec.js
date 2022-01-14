export class demension3_vec
{
    constructor(x, y, z)
    {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    normalize()
    {
        const len = this.getLength();

        this.x /= len;
        this.y /= len;
        this.z /= len;
    }

    // 向量长度标准化
    normalized()
    {
        return this.div(this.getLength());
    }

    getLength()
    {
        return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
    }

    dot(v)
    {
        return this.x * v.x + this.y * v.y + this.z * v.z;
    }

    // 求两向量所在面法线
    cross(v)
    {
        return new demension3_vec(this.y * v.z - this.z * v.y, this.z * v.x - this.x * v.z, this.x * v.y - this.y * v.x);
    }

    add(v)
    {
        return new demension3_vec(this.x + v.x, this.y + v.y, this.z + v.z);
    }

    sub(v)
    {
        return new demension3_vec(this.x - v.x, this.y - v.y, this.z - v.z);
    }

    div(v)
    {
        return new demension3_vec(this.x / v, this.y / v, this.z / v);
    }

    mul(v)
    {
        return new demension3_vec(this.x * v, this.y * v, this.z * v);
    }

    mulXYZ(x, y, z)
    {
        return new demension3_vec(this.x * x, this.y * y, this.z * z);
    }
}