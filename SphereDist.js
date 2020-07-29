// in radians
function TSpherical_Coordinates(col = 0.0, lon = 0.0) {
    this.col = col; // colatitude
    this.lon = lon; // longitude
}

function Tvector2(x = 0.0, y = 0.0) {
    this.x = x;
    this.y = y;
}

function Tvector3(x = 0.0, y = 0.0, z = 0.0) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.add = function(another) {
        this.x += another.x;
        this.y += another.y;
        this.z += another.z;
    }
}

function sqr(x) {
    return x * x;
}

function Spherical2Cartesian(p) {
    return new Tvector3(
        Math.sin(p.col) * Math.cos(p.lon),
        Math.sin(p.col) * Math.sin(p.lon),
        Math.cos(p.col));
}

function Cartesian2Spherical(p) {
    return new TSpherical_Coordinates(
        Math.acos(p.z / Math.sqrt(sqr(p.x) + sqr(p.y) + sqr(p.z))),
        Math.atan2(p.y, p.x));
}

function SphereDist(N = 100, hemisphere = false, relocating = true) {
    this.version = "SphereDist version 0.8.3 by katahiromz";
    this.N = N;
    this.NP_on_hemisphere = N / 2;
    this.hemisphere = hemisphere;
    this.relocating = relocating;
    this.GSS = [];

    this.GetNP_on_hemisphere = function() {
        var NP_on_hemisphere = 0;
        for (var ith_point = 1; ith_point <= this.N; ++ith_point) {
            if (this.GSS[ith_point].col <= Math.PI / 2.0)
                break;
            ++NP_on_hemisphere;
        }
        this.NP_on_hemisphere = NP_on_hemisphere;
        return NP_on_hemisphere;
    }

    this.relocate = function() {
        const five_points = [0xFFFF, 2, 3, 5, 6, 7];

        // relocate 1st point
        var mean = new Tvector3();
        for (var i = 1; i <= 5; ++i) {
            var m = five_points[i];
            mean.add(Spherical2Cartesian(this.GSS[m]));
        }
        this.GSS[1] = Cartesian2Spherical(mean);

        // relocate Nth point
        mean = new Tvector3();
        for (var i = 1; i <= 5; ++i) {
            var m = five_points[i];
            mean.add(Spherical2Cartesian(this.GSS[this.N - m + 1]));
        }
        this.GSS[this.N] = Cartesian2Spherical(mean);
    }

    this.generate = function() {
        if (this.N < 6) {
            throw "N must be >= 6";
        }

        this.GSS = [];
        for (var i = 0; i < this.N + 1; ++i) {
            this.GSS.push(new TSpherical_Coordinates());
        }

        this.GSS[1].lon = 0.0;
        this.GSS[1].col = Math.PI;

        for (var k = 2; k <= this.N - 1; ++k) {
            var hk = -1.0 + 2.0 * (k - 1) / (this.N - 1);
            this.GSS[k].col = Math.acos(hk);
            this.GSS[k].lon = 
                this.GSS[k - 1].lon +
                    3.6 / Math.sqrt(this.N) / Math.sqrt(1.0 - hk * hk);
        }

        this.GSS[this.N].lon = 0.0;
        this.GSS[this.N].col = 0.0;

        if (this.hemisphere) {
            this.GetNP_on_hemisphere();
        }

        if (this.relocating) {
            this.relocate();
        }
    }

    this.rows = function() {
        var vertexes = [];
        const last = this.hemisphere ? this.NP_on_hemisphere : this.N;
        for (var ith_point = 1; ith_point <= last; ++ith_point) {
            var p = Spherical2Cartesian(this.GSS[ith_point]);
            vertexes.push([p.x, p.y, -p.z]);
        }
        return vertexes;
    }

    this.get_text = function() {
        var vertexes = this.rows();
        var text = "";
        for (var i = 0; i < vertexes.length; ++i) {
            text += vertexes[i][0];
            text += ", ";
            text += vertexes[i][1];
            text += ", ";
            text += vertexes[i][2];
            text += "\n";
        }
        return text;
    }

    this.generate();
}
