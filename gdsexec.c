#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <regex.h>


double r642d(uint64_t v){
        if (v == 0) return 0;
        uint64_t vs = (v >> 63) & 0x1;
        uint64_t ve = (v >> 56) & 0x7F;
        uint64_t vm = v & 0x00ffffffffffffff;
        int16_t exp = ((int16_t)ve - 64)*4;
        exp -= 4;
        for (; (vm >> 52) > 1; ++exp, vm >>= 1);
        ve = (uint64_t)(exp + 1023) & 0x7ff;
        uint64_t r = (vs << 63) | (ve << 52) | (vm & 0x000fffffffffffff);
        return *(double*)&r;
}

uint64_t d2r64(double v){
        if (v == 0) return 0;
        uint64_t raw = *(uint64_t*)&v;
        uint64_t vs = (raw >> 63) & 0x1;
        uint64_t ve = (raw >> 52) & 0x7ff;
        uint64_t vm = (1ul << 52) | (raw & 0x000fffffffffffff);
        uint32_t shift = (ve+1) % 4;
        int16_t exp = ((int16_t)ve - 1023);
        vm <<= shift;
        exp += 4;
        exp -= shift;
        ve = (uint64_t)(exp / 4 + 64) & 0x7f;
        return (vs << 63) | (ve << 56) | (vm & 0x00ffffffffffffff);
}

FILE* fp0;
FILE* fp1;

uint8_t u0, u1, length;
uint16_t u00, u2, length1;
int16_t i2;
uint32_t u0000, u4, length2;
uint64_t u8;
double f8;
void rdu1(){ 
	u0 = (uint8_t)(((u2>>8)&0xff)); 
	u00 = (uint16_t)(((uint16_t)((u00<<8)&0xff00)) | ((uint16_t)(u0&0x00ff))); 
	u0000 = (uint32_t)(((uint32_t)((u0000<<8)&0xffffff00)) | ((uint32_t)(u0&0x000000ff))); 
	fread(&u1, sizeof(uint8_t), 1, fp0);
	u2 = (uint16_t)(((uint16_t)((u2<<8)&0xff00)) | ((uint16_t)(u1&0x00ff))); 
	u4 = (uint32_t)(((uint32_t)((u4<<8)&0xffffff00)) | ((uint32_t)(u1&0x000000ff))); 
	u8 = (uint64_t)(((uint64_t)((u8<<8)&0xffffffffffffff00)) | ((uint64_t)(u1&0x00000000000000ff))); 
}
void rdu2(){ rdu1(); rdu1(); }
void rdu4(){ rdu2(); rdu2(); }
void rdu8(){ rdu4(); rdu4(); }

void wtu1(){ fwrite(&u1, sizeof(uint8_t), 1, fp1); }
void wtu2(){ u1 = ((u2>>8)&0xff); wtu1(); u1 = (u2&0xff); wtu1(); }
void wtu4(){ u2 = ((u4>>16)&0xffff); wtu2(); u2 = (u4&0xffff); wtu2(); }
void wtu8(){ u4 = ((u8>>32)&0xffffffff); wtu4(); u4 = (u8&0xffffffff); wtu4(); }

char rstring[1024];
char token[1024];
int cst;

void gds2txt(char* gds, char* txt){
	fp0 = fopen(gds, "rb");
	fp1 = fopen(txt, "w");
	cst = 0;
	int k;
	if(fp0 != NULL){
		printf("read gds file %s\n", gds);
		printf("write txt file %s\n", txt);
		while(!feof(fp0)){
			rdu2();
			if(cst == 0){
				     if(0x0102 == u2){ fprintf(fp1, "RT_BGNLIB "); for(k = 0; k < 12; k++){ rdu2(); fprintf(fp1, " %04x ", u2); } fputc('\n', fp1); cst = 1; }
				else if(0x0206 == u2){ length = u0; fread(rstring, length-4, 1, fp0); rstring[length-4] = '\0'; fprintf(fp1, "RT_LIBNAME \"%s\" \n", rstring); cst = 1; printf(" read lib %s \n", rstring); }
				else if(0x2202 == u2){ rdu2(); fprintf(fp1, "RT_GENERATIONS %d \n", u2); cst = 1; }
				else if(0x0305 == u2){ fprintf(fp1, "RT_UNITS "); for(k = 0; k < 4; k++){ rdu4(); fprintf(fp1, " %08x ", u4); } fputc('\n', fp1); cst = 1; }
				else if(0x0400 == u2){ fprintf(fp1, "RT_ENDLIB\n"); cst = 1; }
				else if(0x0502 == u2){ fprintf(fp1, "RT_BGNSTR "); for(k = 0; k < 6; k++){ rdu4(); fprintf(fp1, " %08x ", u4); } fputc('\n', fp1); cst = 1; }
				else if(0x0700 == u2){ fprintf(fp1, "RT_ENDSTR\n"); cst = 1; }
				else if(0x0606 == u2){ length = u0; fread(rstring, length-4, 1, fp0); rstring[length-4] = '\0'; fprintf(fp1, " RT_STRNAME \"%s\" \n", rstring); cst = 1; printf(" read stream %s \n", rstring); }
				else if(0x0800 == u2){ fprintf(fp1, " RT_BOUNDARY "); cst = 1; }
				else if(0x0900 == u2){ fprintf(fp1, " RT_PATH "); cst = 1; }
				else if(0x0a00 == u2){ fprintf(fp1, " RT_SREF "); cst = 1; }
				else if(0x0b00 == u2){ fprintf(fp1, " RT_AREF "); cst = 1; }
				else if(0x0c00 == u2){ fprintf(fp1, " RT_TEXT "); cst = 1; }
				else if(0x0d02 == u2){ fprintf(fp1, " RT_LAYER "); rdu2(); fprintf(fp1, " %d ", u2); cst = 1; }
				else if(0x0e02 == u2){ fprintf(fp1, " RT_DATATYPE "); rdu2(); fprintf(fp1, " %d ", u2); cst = 1; }
				else if(0x0f03 == u2){ fprintf(fp1, " RT_WIDTH "); rdu4(); fprintf(fp1, " %d ", u4); cst = 1; }
				else if(0x1003 == u2){ length1 = (uint16_t)((u0000>>32)&0xffff); fprintf(fp1, " RT_XY %x ", length1); for(k = 0; k < ((length1-4)/4); k++){ rdu4(); fprintf(fp1, "%d ", (int32_t)u4); } cst = 1; }
				else if(0x1100 == u2){ fprintf(fp1, " RT_ENDEL \n"); cst = 1; }
				else if(0x1206 == u2){ length = u0; fread(rstring, length-4, 1, fp0); rstring[length-4] = '\0'; fprintf(fp1, " RT_SNAME \"%s\" ", rstring); cst = 1; }
				else if(0x1302 == u2){ fprintf(fp1, " RT_COLROW "); rdu4(); fprintf(fp1, " %d ", u4); cst = 1; }
				else if(0x1602 == u2){ fprintf(fp1, " RT_TEXTTYPE "); rdu2(); fprintf(fp1, " %d ", u2); cst = 1; }
				else if(0x1701 == u2){ fprintf(fp1, " RT_PRESENTATION "); rdu2(); fprintf(fp1, " %d ", u2); cst = 1; }
				else if(0x1906 == u2){ length = u0; fread(rstring, length-4, 1, fp0); rstring[length-4] = '\0'; fprintf(fp1, " RT_STRING \"%s\" ", rstring); cst = 1; }
				else if(0x1a01 == u2){ fprintf(fp1, " RT_STRANS "); rdu2(); fprintf(fp1, " %04x ", u2); cst = 1; }
				else if(0x1b05 == u2){ fprintf(fp1, " RT_MAG "); rdu8(); fprintf(fp1, " %lf ", r642d(u8)); cst = 1; }
				else if(0x1c05 == u2){ fprintf(fp1, " RT_ANGLE "); rdu8(); fprintf(fp1, " %lf ", r642d(u8)); cst = 1; }
				else if(0x2102 == u2){ fprintf(fp1, " RT_PATHTYPE "); rdu2(); fprintf(fp1, " %d ", u2); cst = 1; }
				else if(0x2b02 == u2){ fprintf(fp1, " RT_PROPATTR "); rdu2(); fprintf(fp1, " %d ", u2); cst = 1; }
				else if(0x2c06 == u2){ length = u0; fread(rstring, length-4, 1, fp0); rstring[length-4] = '\0'; fprintf(fp1, " RT_PROPVALUE \"%s\" ", rstring); cst = 1; }
			}
			else {
				if((cst == 1) && (0x0000 == (u2 & 0xf000))){ cst = 0; }
			}
		}
	}
	else printf("can not open gds file %s\n", gds);
	fclose(fp0);
	fclose(fp1);
}

void read_rstring(){
	char c;
	int k;
	do c = fgetc(fp0); while(c != '"');
	k = 0;
	do {
		c = fgetc(fp0); 
		if(c != '"') {
			rstring[k] = c;
			k++;
		}
	} while(c != '"');
	rstring[k] = '\0';
}

void txt2gds(char* txt, char* gds){
	int k;
	fp0 = fopen(txt, "r");
	fp1 = fopen(gds, "wb");
	cst = 0;
	if(fp0 != NULL){
		if(cst == 0){
			printf("read txt file %s\n", txt);
			printf("write gds file %s\n", gds);
			u2 = 0x0006; wtu2();
			u2 = 0x0002; wtu2();
			u2 = 0x0005; wtu2();
			while(!feof(fp0)){
				fscanf(fp0, "%s", token);
				if(strcmp(token, "RT_BGNLIB") == 0) {
					u2 = (12*2+4); wtu2(); u2 = 0x0102; wtu2();
					for(k = 0; k < 12; k++){ fscanf(fp0, "%04x", &u2); wtu2(); } 
				}
				else if(strcmp(token, "RT_LIBNAME") == 0) {
					read_rstring();//fscanf(fp0, "%*[\"]%99[^\"]", rstring);
					u2 = (strlen(rstring)+4); if(strlen(rstring) % 2 == 1) u2++; wtu2(); u2 = 0x0206; wtu2();
					for(k = 0; k < strlen(rstring); k++){ u1 = (uint8_t)rstring[k]; wtu1(); } if(k % 2 == 1) fputc(0x00, fp1);
					printf(" write lib %s \n", rstring);
				}
				else if(strcmp(token, "RT_GENERATIONS") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x2202; wtu2(); fscanf(fp0, "%d", &u2); wtu2(); }
				else if(strcmp(token, "RT_UNITS") == 0) {
					u2 = (4*4+4); wtu2(); u2 = 0x0305; wtu2();
					for(k = 0; k < 4; k++){ fscanf(fp0, "%08x", &u4); wtu4(); } 
				}
				else if(strcmp(token, "RT_BGNSTR") == 0) {
					u2 = (6*4+4); wtu2(); u2 = 0x0502; wtu2();
					for(k = 0; k < 6; k++){ fscanf(fp0, "%08x", &u4); wtu4(); } 
				}
				else if(strcmp(token, "RT_STRNAME") == 0) {
					read_rstring();//fscanf(fp0, "%*[\"]%99[^\"]", rstring);
					u2 = (strlen(rstring)+4); if(strlen(rstring) % 2 == 1) u2++; wtu2(); u2 = 0x0606; wtu2();
					for(k = 0; k < strlen(rstring); k++){ u1 = (uint8_t)rstring[k]; wtu1(); } if(k % 2 == 1) fputc(0x00, fp1);
					printf(" write stream %s \n", rstring);
				}
				else if(strcmp(token, "RT_BOUNDARY") == 0) { u2 = 4; wtu2(); u2 = 0x0800; wtu2(); }
				else if(strcmp(token, "RT_PATH") == 0) { u2 = 4; wtu2(); u2 = 0x0900; wtu2(); }
				else if(strcmp(token, "RT_TEXT") == 0) { u2 = 4; wtu2(); u2 = 0x0c00; wtu2(); }
				else if(strcmp(token, "RT_SREF") == 0) { u2 = 4; wtu2(); u2 = 0x0a00; wtu2(); }
				else if(strcmp(token, "RT_AREF") == 0) { u2 = 4; wtu2(); u2 = 0x0b00; wtu2(); }
				else if(strcmp(token, "RT_LAYER") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x0d02; wtu2(); fscanf(fp0, "%d", &u2); wtu2(); }
				else if(strcmp(token, "RT_DATATYPE") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x0e02; wtu2(); fscanf(fp0, "%d", &u2); wtu2(); }
				else if(strcmp(token, "RT_COLROW") == 0) { u2 = (2*2+4); wtu2(); u2 = 0x1302; wtu2(); fscanf(fp0, "%d", &u4); wtu4(); }
				else if(strcmp(token, "RT_TEXTTYPE") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x1602; wtu2(); fscanf(fp0, "%d", &u2); wtu2(); }
				else if(strcmp(token, "RT_PATHTYPE") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x2102; wtu2(); fscanf(fp0, "%d", &u2); wtu2(); }
				else if(strcmp(token, "RT_PRESENTATION") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x1701; wtu2(); fscanf(fp0, "%d", &u2); wtu2(); }
				else if(strcmp(token, "RT_PROPATTR") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x2b02; wtu2(); fscanf(fp0, "%d", &u2); wtu2(); }
				else if(strcmp(token, "RT_WIDTH") == 0) { u2 = (1*4+4); wtu2(); u2 = 0x0f03; wtu2(); fscanf(fp0, "%d", &u4); wtu4(); }
				else if(strcmp(token, "RT_STRANS") == 0) { u2 = (1*2+4); wtu2(); u2 = 0x1a01; wtu2(); fscanf(fp0, "%04x", &u2); wtu2(); }
				else if(strcmp(token, "RT_MAG") == 0) { u2 = (1*8+4); wtu2(); u2 = 0x1b05; wtu2(); fscanf(fp0, "%lf", &f8); u8 = d2r64(f8); wtu8(); }
				else if(strcmp(token, "RT_ANGLE") == 0) { u2 = (1*8+4); wtu2(); u2 = 0x1c05; wtu2(); fscanf(fp0, "%lf", &f8); u8 = d2r64(f8); wtu8(); }
				else if(strcmp(token, "RT_STRING") == 0) {
					read_rstring();//fscanf(fp0, "%*[\"]%99[^\"]", rstring);
					u2 = (strlen(rstring)+4); if(strlen(rstring) % 2 == 1) u2++; wtu2(); u2 = 0x1906; wtu2();
					for(k = 0; k < strlen(rstring); k++){ u1 = (uint8_t)rstring[k]; wtu1(); } if(k % 2 == 1) fputc(0x00, fp1);
				}
				else if(strcmp(token, "RT_SNAME") == 0) {
					read_rstring();//fscanf(fp0, "%*[\"]%99[^\"]", rstring);
					u2 = (strlen(rstring)+4); if(strlen(rstring) % 2 == 1) u2++; wtu2(); u2 = 0x1206; wtu2();
					for(k = 0; k < strlen(rstring); k++){ u1 = (uint8_t)rstring[k]; wtu1(); } if(k % 2 == 1) fputc(0x00, fp1);
				}
				else if(strcmp(token, "RT_PROPVALUE") == 0) {
					read_rstring();//fscanf(fp0, "%*[\"]%99[^\"]", rstring);
					u2 = (strlen(rstring)+4); if(strlen(rstring) % 2 == 1) u2++; wtu2(); u2 = 0x2c06; wtu2();
					for(k = 0; k < strlen(rstring); k++){ u1 = (uint8_t)rstring[k]; wtu1(); } if(k % 2 == 1) fputc(0x00, fp1);
				}
				else if(strcmp(token, "RT_XY") == 0) {
					fscanf(fp0, "%x", &length1);
					u2 = (uint16_t)length1; wtu2(); u2 = 0x1003; wtu2();
					for(k = 0; k < ((length1-4)/4); k++){ fscanf(fp0, "%d", &u4); wtu4(); } 
				}
				else if(strcmp(token, "RT_ENDEL") == 0) { u2 = 4; wtu2(); u2 = 0x1100; wtu2(); }
				else if(strcmp(token, "RT_ENDSTR") == 0) { u2 = 4; wtu2(); u2 = 0x0700; wtu2(); }
				else if(strcmp(token, "RT_ENDLIB") == 0) { u2 = 4; wtu2(); u2 = 0x0400; wtu2(); }
			}
		}
	}
	else printf("can not open txt file %s\n", txt);
	fclose(fp0);
	fclose(fp1);
}

int main(int argc, char** argv){
	int mode;
	sscanf(argv[1], "%d", &mode);
	switch(mode){
		case 0: if(argc == 4) gds2txt(argv[2], argv[3]); break;
		case 1: if(argc == 4) txt2gds(argv[2], argv[3]); break;
		default: break;
	}
	return 0;
}
