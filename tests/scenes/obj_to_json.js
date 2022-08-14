const { readFileSync } = require("fs");
const contents = readFileSync(process.argv[2], { encoding: "utf-8" });

const vertexes = [];
const normals = [];
const faces = [];

contents.split(/\r?\n/g).forEach((line) => {
  if (line.startsWith("v")) {
    const vs = line
      .slice(2)
      .split(/ /g)
      .filter((l) => l.trim().length !== 0)
      .map((s) => Number(s.trim()));

    if (vs.length !== 3) {
      throw new Error(`bruh ${vs.length}`);
    }
    vertexes.push(vs);
  } else if (line.startsWith("vn")) {
    const vn = line
      .slice(2)
      .split(/ /g)
      .filter((l) => l.trim().length !== 0)
      .map((s) => Number(s.trim()));

    if (vn.length !== 3) {
      throw new Error(`bruh ${vn.length}`);
    }
    normals.push(vn);
  } else if (line.startsWith("f ")) {
    const face = line
      .slice(2)
      .split(/ /g)
      .filter((l) => l.trim().length !== 0)
      .map((s) => s.split("//").map((n) => Number(n)));

    if (face.length !== 3 && face.length !== 4) {
      throw new Error(`bruh ${face.length}`);
    }
    faces.push(face);
  }
});

const data = {
  camera: {
    look_from: [0, 1, 10],
    look_at: [-0.3, 0.5, 0],
    vup: [0, 1, 0],
    vfov: 20,
    aperture: 0,
  },
  materials: {
    blue: {
      type: "lambertian",
      albedo: [0, 0.5, 1],
    },
    glass: {
      type: "dielectric",
      refractive_index: 1.52,
      attenuation: [1, 1, 1],
    },
    gray: {
      type: "lambertian",
      albedo: [0.1, 0.1, 0.1],
    },
    light: {
      type: "light",
      emittance: [23.8635 / 8, 17.708 / 8, 5.6817 / 8],
    },
    bright: {
      type: "light",
      emittance: [23.8635, 17.708, 5.6817],
    },
  },
  objects: [
    {
      name: "monkey",
      material: "blue",
      primitives: [],
    },
    {
      name: "bright light",
      material: "bright",
      primitives: [],
    },
    {
      name: "floor",
      material: "gray",
      primitives: [
        {
          type: "triangle",
          normal: 1,
          vertices: [
            [10000, -1, 10000],
            [10000, -1, -10000],
            [-10000, -1, -10000],
          ],
        },
        {
          type: "triangle",
          normal: 1,
          vertices: [
            [10000, -1, 10000],
            [-10000, -1, -10000],
            [-10000, -1, 10000],
          ],
        },
      ],
    },
    {
      name: "light",
      material: "light",
      primitives: [
        {
          type: "sphere",
          origin: [0, 280, 0],
          radius: 200.0,
        },
      ],
    },
  ],
};

for (let i = 0; i < faces.length; i++) {
  if (faces[i].length === 3) {
    data.objects[0].primitives.push({
      type: "triangle",
      vertices: [
        vertexes[faces[i][0][0] - 1],
        vertexes[faces[i][1][0] - 1],
        vertexes[faces[i][2][0] - 1],
      ],
      normal: 1,
    });
  } else {
    data.objects[0].primitives.push({
      type: "triangle",
      vertices: [
        vertexes[faces[i][0][0] - 1],
        vertexes[faces[i][1][0] - 1],
        vertexes[faces[i][2][0] - 1],
      ],
      normal: 1,
    });
    data.objects[0].primitives.push({
      type: "triangle",
      vertices: [
        vertexes[faces[i][0][0] - 1],
        vertexes[faces[i][2][0] - 1],
        vertexes[faces[i][3][0] - 1],
      ],
      normal: 1,
    });
  }
}

for (let i = 0; i < 10; i++) {
  const radius = 0.1;
  const angle = (i / 10) * 2 * Math.PI;
  data.objects[1].primitives.push({
    type: "sphere",
    origin: [Math.sin(angle) - 0.3, radius - 1, Math.cos(angle)],
    radius,
  });
}

console.log(JSON.stringify(data, null, 2));
