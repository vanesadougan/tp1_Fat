#include <stdio.h>
#include <stdlib.h>
#define START_FIRST_PARTITION_TABLE 446

typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char start_sector[4];
    char length_sectors[4];
} __attribute((packed)) PartitionTable;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size; // 2 bytes
    char clusterSectors;
    unsigned short reserved_sectors;
    char q_of_fats;
    unsigned short root_directory_entries;
    char number_of_sector[2];
    char media_type;
    unsigned short fat_size_sectors;
    char sectors_per_track[2];
    char number_of_heads[2];
    char sectors_b4_start[4];
    char sectors_in_fs[4];
    char bios_int;
    char not_used;
    char ext_boot;
    unsigned int volume_id;
    char volume_label[11];
    char fs_type[8]; // Type en ascii
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    // Ir al inicio de la tabla de particiones.
    fseek(in, START_FIRST_PARTITION_TABLE , SEEK_SET);  
    fread(pt, sizeof(PartitionTable), 4, in);

    for(i=0; i<4; i++) {
        printf("Partition type: %d\n", pt[i].partition_type);
        if(pt[i].partition_type == 1) {
            printf("Encontrado FAT12 %d\n", i);
            break;
        }
    }
    if(i == 4) {
        printf("No se encontró filesystem FAT12, saliendo ...\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);

    printf("  Jump code: %02X:%02X:%02X\n", bs.jmp[0], bs.jmp[1], bs.jmp[2]);
    printf("  OEM code: [%.8s]\n", bs.oem);
    printf("  sector_size: %d\n", bs.sector_size);
//Agregado:
   printf("  Fat Size Sectors: %d\n", bs.fat_size_sectors);
   printf("  Quantity of FATs: %d\n", bs.q_of_fats);
   printf("  Reserved Sectors: %d\n", bs.reserved_sectors);
   printf("  Root Directory Entries: %d\n", bs.root_directory_entries);
//Fin
    printf("  volume_id: 0x%08X\n", bs.volume_id);
    printf("  Volume label: [%.11s]\n", bs.volume_label);
    printf("  Filesystem type: [%.8s]\n", bs.fs_type);
    printf("  Boot sector signature: 0x%04X\n", bs.boot_sector_signature);

    fclose(in);
    return 0;
}
