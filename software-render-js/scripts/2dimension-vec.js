export class demension2_vec
{
    constructor(x, y)
    {
        this.x = x;
        this.y = y;
    }

    dot(v)
    {
        return this.x * v.x + this.y * v.y;
    }

    cross(v)
    {
        return this.y * v.x - this.x * v.y;
    }

    add(v)
    {
        return new demension2_vec(this.x + v.x, this.y + v.y);
    }

    sub(v)
    {
        return new demension2_vec(this.x - v.x, this.y - v.y);
    }

    div(v)
    {
        return new demension2_vec(this.x / v, this.y / v);
    }

    mul(v)
    {
        return new demension2_vec(this.x * v, this.y * v);
    }
}