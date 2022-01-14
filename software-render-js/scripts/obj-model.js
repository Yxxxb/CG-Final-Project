import { ObjFace } from "./obj-face.js";
import { demension2_vec } from "./2dimension-vec.js";
import { demension3_vec } from "./3demension-vec.js";
import { ColorPoint } from "./color-point.js";

export class ObjModel
{
    constructor(vPositions, vTexCoords, vNormals, indices)
    {
        this.vPositions = vPositions;
        this.vTexCoords = vTexCoords;
        this.vNormals = vNormals;
        this.indices = indices;
        this.faces = [];

        for (let i = 0; i < this.indices.length; i++)
        {
            let vFace = this.indices[i];

            let face = [];
            for (let v = 0; v < 3; v++)
            {
                const pos = this.getPosition(vFace[v][0] - 1);
                const tex = this.getTexCoord(vFace[v][1] - 1);
                const nor = this.getNormal(vFace[v][2] - 1);
                face.push(new ColorPoint(pos, 0xffffff, tex, nor));
            }

            face = new ObjFace(face[0], face[1], face[2]);
            face.calcTangentAndBiTangent();

            this.faces.push(face);
        }
    }

    getPosition(pos)
    {
        return new demension3_vec(this.vPositions[pos][0], this.vPositions[pos][1], this.vPositions[pos][2]);
    }

    getTexCoord(tex)
    {
        return new demension2_vec(this.vTexCoords[tex][0], this.vTexCoords[tex][1]);
    }

    getNormal(nor)
    {
        return new demension3_vec(this.vNormals[nor][0], this.vNormals[nor][1], this.vNormals[nor][2]);
    }
}
