#ifndef EASY_OBJ_H_
#define EASY_OBJ_H_

typedef struct {
	int *vi, *vti, *vni;
	int cnt;
} EasyOBJ_Face;

typedef struct {
	float x, y, z;
} EasyOBJ_V3;

typedef struct {
	float x, y;
} EasyOBJ_V2;

typedef struct {
	EasyOBJ_V3 *vs, *vns;
	EasyOBJ_V2 *vts;
	EasyOBJ_Face *fs;
	int vn, vnn, vtn, fn;
} EasyOBJ;

EasyOBJ easy_obj_parse(const char *filename);
void    easy_obj_free(EasyOBJ obj);

#endif

#ifdef EASY_OBJ_IMPLEMENTATION

int easy_obj_split(
	const char *str,
	const char *delimiter,
	char *buffer, size_t buffer_size,
	char **tokens, int max_tokens
) {
	size_t len = strlen(str);
	if (len + 1 > buffer_size) return -1;

	memcpy(buffer, str, len + 1);

	char *saveptr = NULL;
	char *tok = strtok_r(buffer, delimiter, &saveptr);
	int idx = 0;

	while (tok != NULL) {
		if (idx >= max_tokens) return -2;
		tokens[idx++] = tok;
		tok = strtok_r(NULL, delimiter, &saveptr);
	}

	return idx;
}

EasyOBJ easy_obj_parse(const char *filename) {
	int vn = 0, vtn = 0, vnn = 0, fn = 0;

	FILE *file = fopen(filename, "r");
	char line[1024 * 16];

	while (fgets(line, sizeof(line), file)) {
		char *ep;
		char *pl[32];
		char plb[1024];
		easy_obj_split(line, " ", plb, sizeof(plb), pl, 32);

		if(!strcmp(pl[0], "v")){
			vn++;
		} else if(!strcmp(pl[0], "vn")){
			vnn++;
		} else if(!strcmp(pl[0], "vt")){
			vtn++;
		} if(!strcmp(pl[0], "f")){
			fn++;
		}
	}

	fclose(file);

	EasyOBJ obj = {
		.vs  = malloc(sizeof(EasyOBJ_V3)*vn),
		.vts = malloc(sizeof(EasyOBJ_V3)*vtn),
		.vns = malloc(sizeof(EasyOBJ_V3)*vnn),
		.fs  = malloc(sizeof(EasyOBJ_Face)*fn),
	};

	file = fopen(filename, "r");
	while (fgets(line, sizeof(line), file)) {
		char *ep;
		char *pl[32];
		char plb[1024];
		int cnt = easy_obj_split(line, " ", plb, sizeof(plb), pl, 32);

		if(!strcmp(pl[0], "v")){
			EasyOBJ_V3 v = {strtof(pl[1], &ep), strtof(pl[2], &ep), strtof(pl[3], &ep)};
			obj.vs[obj.vn++] = v;
		} else if(!strcmp(pl[0], "vn")){
			EasyOBJ_V3 n = {strtof(pl[1], &ep), strtof(pl[2], &ep), strtof(pl[3], &ep)};
			obj.vns[obj.vnn++] = n;
		} else if(!strcmp(pl[0], "vt")){
			EasyOBJ_V2 uv = {strtof(pl[1], &ep), strtof(pl[2], &ep)};
			obj.vts[obj.vtn++] = uv;
		} else if(!strcmp(pl[0], "f")){
			EasyOBJ_Face f = {
				.vi  = malloc(sizeof(int)*(cnt-1)),
				.vti = malloc(sizeof(int)*(cnt-1)),
				.vni = malloc(sizeof(int)*(cnt-1)),
			};

			for(int i = 1; i < cnt; i++){
				char *pf[32];
				char pfb[1024];
				int fn = easy_obj_split(pl[i], "/", pfb, sizeof(pfb), pf, 32);

				if(fn == 2){
					f.vi[f.cnt]    = strtol(pf[0], &ep, 10);
					f.vni[f.cnt++] = strtol(pf[1], &ep, 10);
				} else {
					f.vi[f.cnt]    = strtol(pf[0], &ep, 10);
					f.vti[f.cnt]   = strtol(pf[1], &ep, 10);
					f.vni[f.cnt++] = strtol(pf[2], &ep, 10);
				}
			}

			obj.fs[obj.fn++] = f;
		}
	}

	fclose(file);
	return obj;
}

void easy_obj_free(EasyOBJ obj) {
	for (size_t i = 0; i < obj.fn; i++) {
		free(obj.fs[i].vi);
		free(obj.fs[i].vti);
		free(obj.fs[i].vni);
	}

	free(obj.fs);
	free(obj.vs);
	free(obj.vns);
	free(obj.vts);
}

#endif
