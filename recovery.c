#include <stdio.h>
#include <stdlib.h>
#define START_FIRST_PARTITION_TABLE 446

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
    unsigned short bytes_per_sector; //Mapeo de 1 cluster ->  tamanio unidad logica del disco
    char clusterSectors;
    unsigned short reserved_sectors;
    char q_of_fats;
    unsigned short max_root_dir_entries;
    char number_of_sector[2];
    char media_type;
    unsigned short fat_size_sectors; //Cantidad de sectores en un fat
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
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct {
	unsigned char filename[8];             
    unsigned char extension[3];        
    unsigned char attributes;          
    unsigned char reserved;            
    unsigned char create_time[3];      
    unsigned char create_date[2];      
    unsigned char last_access_date[2]; 
    unsigned short first_cluster_msb;  
    unsigned char last_mod_time[2];    
    unsigned char last_mod_date[2];    
    unsigned short first_cluster_lsb;  
    unsigned int file_size;            
} __attribute((packed)) Fat12Entry;

void recover_file(Fat12Entry *entry, int file_position){
    //Reemplazo por 'A' (o cualquier letra) para que deje ver el archivo como si estuvise borrado
    printf("Entre al recover");
    printf("Size del fat12entry %d", sizeof(Fat12Entry));
    entry->filename[0] = "A";
    FILE *file = fopen("test.img", "r+");
    fseek(file, file_position - sizeof(Fat12Entry), SEEK_SET);
    int position = ftell(file);
    printf("position %d\n", position);
    fwrite(entry, sizeof(Fat12Entry), 1, file);
    fclose(file);
    printf("Archivo recuperado %s\n", entry->filename);
}

void print_file_info(Fat12Entry *entry, int file_position) {
    switch(entry->filename[0]) {
    case 0x00:
        return;
    //Solo se recueran los archivos que comienzan 0xE5
    //Como aclaramos en los otros files, un archivo 0x05 tambien esta borrado. Pero esta en una lista de archivos por borrar 
    case 0XE5: 
        printf("Archivo borrado: [?%.8s.%.3s]\n", entry->filename, entry->extension);
        recover_file(entry, file_position);
        return;
    default:
        if (entry->attributes == 0x10){
            printf("Directorio: [%.8s.%.3s]\n", entry->filename, entry->extension, entry->file_size); 
            return;
        }
        else if (entry->attributes == 0x20){
            printf("Archivo: [%.8s.%.3s]\n", entry->filename, entry->extension); 
        }
    }
}

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;

    fseek(in, START_FIRST_PARTITION_TABLE, SEEK_SET);
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
           ftell(in), bs.bytes_per_sector, bs.fat_size_sectors, bs.q_of_fats);

    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.q_of_fats) *
          bs.bytes_per_sector, SEEK_CUR);

    printf("Root dir_entries %d \n", bs.max_root_dir_entries);
    for(i=0; i<bs.max_root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);
        print_file_info(&entry, ftell(in));
    }

    printf("\nLeido Root directory, ahora en 0x%X\n", ftell(in));
    fclose(in);
    return 0;
}
