#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "desmos3D.h"

#define EASY_OBJ_IMPLEMENTATION
#include "../3rdparty/easy_obj.h"

char *read_file(const char *filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long filesize = ftell(file);
	rewind(file);

	char *buffer = malloc(filesize + 1);
	if (!buffer) {
		fclose(file);
		return NULL;
	}

	size_t read_size = fread(buffer, 1, filesize, file);
	if (read_size != filesize) {
		free(buffer);
		fclose(file);
		return NULL;
	}

	buffer[filesize] = '\0';

	fclose(file);
	return buffer;
}

int write_to_file(const char *filename, const char *text) {
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		perror("Error opening file");
		return -1;
	}

	if (fputs(text, file) == EOF) {
		perror("Error writing to file");
		fclose(file);
		return -1;
	}

	if (fclose(file) == EOF) {
		perror("Error closing file");
		return -1;
	}

	return 0;
}

void write_vec3(char *buf, EasyOBJ_V3 v) {
	char vbuf[64];
	sprintf(vbuf, "(%f, %f, %f)", v.x, v.y, v.z);
	strcat(buf, vbuf);
}

void write_triangle(char *buf, int a, int b, int c) {
	const char *base = "{\"type\":\"expression\",\"color\":\"#000000\",\"latex\":\"\\\\operatorname{polygon}((Q[%i].x,Q[%i].y),(Q[%i].x,Q[%i].y),(Q[%i].x,Q[%i].y))\"},";
	char vbuf[256];
	sprintf(vbuf, base, a, a, b, b, c, c);
	strcat(buf, vbuf);
}

void render_model_to_desmos(EasyOBJ rm, const char *output) {
	char res[1024 * 1024];
	char vbuf[1024 * 1024];
	char tbuf[1024 * 1024];

	for (int i = 0; i < rm.vn; i++) {
		write_vec3(vbuf, rm.vs[i]);
		if (i < rm.vn - 1) strcat(vbuf, ",");
	}

	for(int fi = 0; fi < rm.fn; fi++){
		int p1 = rm.fs[fi].vi[0];
		for(int vi = 1; vi < rm.fs[fi].cnt-1; vi++){
			int p2 = rm.fs[fi].vi[vi];
			int p3 = rm.fs[fi].vi[vi+1];
			write_triangle(tbuf, p1, p2, p3);
		}
	}

	sprintf(res, desmos3D, vbuf, tbuf);
	write_to_file(output, res);
}

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: input.obj output.json");
		return 1;
	}

	EasyOBJ rm = easy_obj_parse(argv[1]);
	render_model_to_desmos(rm, argv[2]);
	return 0;
}
