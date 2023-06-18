//
// Created by cmtheit on 23-6-18.
//
#include <elf.h>
#include <stdio.h>

static inline void report_error() {
    printf("Not recognized as ELF file\n");
}

typedef enum ELFCls {
    C_None,
    C_EL32,
    C_EL64,
} ELFCls;

typedef enum ELFData {
    D_None,
    D_LE,
    D_BE
} ELFData;

typedef enum ELFVersion {
    V_None,
    V_Crt
} ELFVersion;

typedef enum ELFOsAbi {
    OA_SYSV,
    OA_HPUX,
    OA_NETBSD,
    OA_LINUX,
    OA_SOLARIS,
    OA_AIX,
    OA_IRIX,
    OA_FREEBSD,
    OA_TRU64,
    OA_MODESTO,
    OA_OPENBSD,
    OA_ARM_AEABI,
    OA_ARM,
    OA_ALONE,
} ELFOsAbi;

typedef struct MyELF {
    ELFCls cls;
    ELFData data;
    ELFVersion version;
    ELFOsAbi abi;
} MyELF;

void myelf_report(MyELF elf) {
    printf("class: ");
    switch (elf.cls) {
        case C_None:
            printf("none");
            break;
        case C_EL32:
            printf("32 bit");
            break;
        case C_EL64:
            printf("64 bit");
            break;
    }
    printf("\n");
    printf("data: ");
    switch (elf.data) {
        case D_None:
            printf("none");
            break;
        case D_BE:
            printf("big endian");
            break;
        case D_LE:
            printf("little endian");
            break;
    }
    printf("\n");
    printf("version: ");
    switch (elf.version) {
        case V_None:
            printf("none");
            break;
        case V_Crt:
            printf("current");
            break;
    }
    printf("\n");
    printf("abi: ");
    switch (elf.abi) {
        case OA_AIX:
            printf("IBM AIX");
            break;
        case OA_ALONE:
            printf("Standalone (embedded) application");
            break;
        case OA_ARM:
            printf("ARM");
            break;
        case OA_ARM_AEABI:
            printf("ARM EABI");
            break;
        case OA_FREEBSD:
            printf("FreeBSD.");
            break;
        case OA_HPUX:
            printf("HP-UX");
            break;
        case OA_IRIX:
            printf("SGI Irix.");
            break;
        case OA_LINUX:
            printf("Linux, Object uses GNU ELF extensions.");
            break;
        case OA_MODESTO:
            printf("Novell Modesto.");
            break;
        case OA_NETBSD:
            printf("NetBSD.");
            break;
        case OA_OPENBSD:
            printf("OpenBSD.");
            break;
        case OA_SOLARIS:
            printf("Sun Solaris.");
            break;
        case OA_SYSV:
            printf("Alias. UNIX System V ABI");
            break;
        case OA_TRU64:
            printf("Compaq TRU64 UNIX.");
            break;
    }
    printf("\n");
}

void report(FILE * file) {
    uint8_t buf[9];
    if (fread(buf, 1, 9, file) != 9) {
        report_error();
        return;
    }
    if (buf[EI_MAG0] == ELFMAG0 && buf[EI_MAG1] == ELFMAG1
    && buf[EI_MAG2] == ELFMAG2 && buf[EI_MAG3] == ELFMAG3) {
        struct MyELF elf;
        switch (buf[4]) {
            case ELFCLASSNONE:
            case ELFCLASS32:
            case ELFCLASS64:
                elf.cls = buf[4];
                break;
            default:
                report_error();
                return;
        }
        switch (buf[5]) {
            case ELFDATANONE:
            case ELFDATA2LSB:
            case ELFDATA2MSB:
                elf.data = buf[5];
                break;
            default:
                report_error();
                return;
        }
        switch (buf[6]) {
            case EV_NONE:
            case EV_CURRENT:
                elf.version = buf[6];
                break;
            default:
                report_error();
                return;
        }
        switch (buf[7]) {
            case ELFOSABI_SYSV:
            case ELFOSABI_HPUX:
            case ELFOSABI_NETBSD:
            case ELFOSABI_LINUX:
            case ELFOSABI_SOLARIS:
            case ELFOSABI_AIX:
            case ELFOSABI_IRIX:
            case ELFOSABI_FREEBSD:
            case ELFOSABI_TRU64:
            case ELFOSABI_MODESTO:
            case ELFOSABI_OPENBSD:
            case ELFOSABI_ARM_AEABI:
            case ELFOSABI_ARM:
            case ELFOSABI_STANDALONE:
                elf.abi = buf[7];
                break;
            default:
                report_error();
                return;
        }
        if (buf[8] != 0) {
            report_error();
            return;
        }
        printf("ELF ok\n");
        myelf_report(elf);
    } else {
        report_error();
    }
}

int main(int argc, char * argv[]) {
    for (int i = 1; i < argc; ++i) {
        FILE * file = fopen(argv[i], "r");
        if (file) {
            printf("%s:\n", argv[i]);
            report(file);
            fclose(file);
        } else {
            printf("文件 %s 无法打开\n", argv[i]);
        }
    }
}