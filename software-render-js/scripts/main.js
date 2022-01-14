'use strict'

import { ConstantsList } from "./constants-list.js";
import { StartGame } from "./start-game.js";
import { ObjModel } from "./obj-model.js";
import * as Resources from "./image-obj-resources.js";

window.onload = () =>
{
    new StartGame().start();
}

// 加载导入obj模型
for (const key in Resources.models)
{
    if (Object.hasOwnProperty.call(Resources.models, key))
    {
        const modelURL = Resources.models[key];

        let xhr = new XMLHttpRequest();
        xhr.open("get", modelURL, true);
        xhr.send(null);

        xhr.onreadystatechange = function ()
        {
            if (xhr.readyState == 4 && xhr.status == 200)
            {
                // Load OBJ file line by line
                const lines = xhr.response.split('\n');

                let positions = [];
                let texCoords = [];
                let normals = [];
                let indices = [];

                for (const line of lines)
                {
                    const tokens = line.split(" ");
                    // console.log(tokens)
                    switch (tokens[0])
                    {
                        case "v":
                            let v = [];
                            for (let i = 0; i < 3; i++)
                                v.push(parseFloat(tokens[i + 1]))
                            positions.push(v);
                            break;

                        case "vt":
                            let tc = [];
                            for (let i = 0; i < 2; i++)
                                tc.push(parseFloat(tokens[i + 1]))
                            texCoords.push(tc);
                            break;

                        case "vn":
                            let vn = [];
                            for (let i = 0; i < 3; i++)
                                vn.push(parseFloat(tokens[i + 1]))
                            normals.push(vn);
                            break;

                        case "f":
                            let f = [];
                            for (let i = 0; i < 3; i++)
                            {
                                let v = [];
                                for (let j = 0; j < 3; j++)
                                    v.push(parseInt(tokens[i + 1].split("/")[j]))
                                f.push(v);
                            }
                            indices.push(f);
                            break;
                    }
                }

                // console.log(indices);

                ConstantsList.loadedResources++;

                // 写进了Model后暂存到resource中 view中会利用drawModel进行模型绘制
                Resources.models[key] = new ObjModel(positions, texCoords, normals, indices);

            }
        }
    }
}

// console.log(Resources.models)