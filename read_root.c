#include <stdio.h>
#include <stdlib.h>
#define PARTITIONTABLEOFFSET 446

typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char starting_cluster[4];
    char file_size[4];
} __attribute((packed)) PartitionTable;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size;
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
    char fs_type[8];
    char boot_code[448]
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct {
	// {...} COMPLETAR que va aca?
} __attribute((packed)) Fat12Entry;

void print_file_info(Fat12Entry *entry, int file_pos, int data_area, int cluster_size) {
    switch(entry->filename[0]) {
    case 0x00:
        return; // unused entry
    case 0x05: // Completar los ...
        printf("Archivo borrado: [?%.7s.%.3s]\n", entry->filename, entry->extension, file_pos); // COMPLETAR
        return;
    case 0xE5: // Completar los ...
        printf("Archivo que comienza con 0xE5: [%c%.7s.%.3s]\n", 0xE5, entry->filename, entry->extension, file_pos); // COMPLETAR
        break;
    case 0x10: // Completar los ...
        printf("Directorio: [%.8s.%.3s]\n", // COMPLETAR
        break;
    default:
        printf("Archivo: [%.8s.%.3s]\n", entry->filename, entry->extension, file_pos); // COMPLETAR
    }

}

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;

    fseek(in, PARTITIONTABLEOFFSET, SEEK_SET);
    fread(pt, sizeof(PartitionTable), 4, in);
    for(i=0; i<4; i++) {
        if(pt[i].partition_type == 1) {
            printf("Encontrada particion FAT12 %d\n", i);
            break;
        }
    }

    if(i == 4) {
        printf("No encontrado filesystem FAT12, saliendo...\n");
        return -1;
    }

    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);

    printf("En  0x%X, sector size %d, FAT size %d sectors, %d FATs\n\n",
           ftell(in), bs.sector_size, bs.fat_size_sectors, bs.number_of_fats);

    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) *
          bs.sector_size, SEEK_CUR);

    printf("Root dir_entries %d \n", bs.root_dir_entries);
    for(i=0; i<bs.root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);
        print_file_info(&entry);
    }

    printf("\nLeido Root directory, ahora en 0x%X\n", ftell(in));
    fclose(in);
    return 0;
}
