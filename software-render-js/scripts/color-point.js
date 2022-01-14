import {demension2_vec} from "./2dimension-vec.js";
import {demension3_vec} from "./3demension-vec.js";

export class ColorPoint {
    constructor(pos, color, texCoord, normal, tangent, biTangent) {
        this.pos = pos;

        if (typeof color == "number") this.color = new demension3_vec((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
        else if (color == undefined) this.color = new demension3_vec(255, 0, 255);
        else this.color = color;

        if (texCoord == undefined) this.texCoord = new demension2_vec(0, 0);
        else this.texCoord = texCoord;

        this.normal = normal;
        this.tangent = tangent;
        this.biTangent = biTangent;
    }
}