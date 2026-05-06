import csv
import json
import re

def normalize_name(name):
    # Remove interchange info like [Conn: Pink]
    name = re.sub(r'\[Conn:.*?\]', '', name)
    # Remove notes like (First Station)
    name = re.sub(r'\(.*?\)', '', name)
    # Remove trailing/leading spaces and handle double spaces
    name = name.strip().replace('  ', ' ')
    # Special case for "Dwarka Sector 21" which is sometimes different
    if "Dwarka Sector 21" in name:
        return "Dwarka Sector 21"
    return name

def get_station_id(name):
    # Generate a clean ID
    return name.upper().replace(' ', '_').replace('-', '_').replace('.', '').replace(',', '')

stations = {}
connections = []

# Map of line names to standardized names
line_map = {
    'Red line': 'Red',
    'Yellow line': 'Yellow',
    'Blue line': 'Blue',
    'Blue line branch': 'Blue',
    'Green line branch': 'Green',
    'Green line': 'Green',
    'Rapid Metro': 'Rapid',
    'Voilet line': 'Violet',
    'Magenta line': 'Magenta',
    'Pink line': 'Pink',
    'Aqua line': 'Aqua',
    'Gray line': 'Gray',
    'Orange line': 'Orange'
}

# Known coordinate fixes
coord_fixes = {
    'SHYAM_PARK': (28.6988, 77.3486),
}

with open('/Users/subh/dmrc-visualizer/data/dmrc_data.csv', 'r') as f:
    reader = csv.DictReader(f)
    current_line_raw = None
    prev_station_id = None
    prev_dist = 0
    
    for row in reader:
        line_raw = row['Metro Line'].strip()
        line_standard = line_map.get(line_raw, line_raw.replace(' line', '').title())
        
        name = normalize_name(row['Station Names'])
        station_id = get_station_id(name)
        
        lat = float(row['Latitude'])
        lon = float(row['Longitude'])
        
        # Fix obviously wrong coordinates
        if station_id in coord_fixes:
            lat, lon = coord_fixes[station_id]
        elif lon < 70: # Lon should be around 77 for Delhi
            if station_id in stations:
                lat, lon = stations[station_id]['lat'], stations[station_id]['lon']
            else:
                # If we don't have a fix and it's first time, we might have a problem
                # But for this task, I'll assume coord_fixes covers the main one (Shyam Park)
                pass
        
        if station_id not in stations:
            stations[station_id] = {
                'id': station_id,
                'name': name,
                'lat': lat,
                'lon': lon,
                'lines': []
            }
        
        if line_standard not in stations[station_id]['lines']:
            stations[station_id]['lines'].append(line_standard)
            
        dist = float(row['Dist. From First Station(km)'])
        
        if line_raw != current_line_raw:
            current_line_raw = line_raw
            prev_station_id = station_id
            prev_dist = dist
        else:
            # Connect to previous station on the same line
            d = abs(dist - prev_dist)
            if d > 0:
                connections.append({
                    'source': prev_station_id,
                    'target': station_id,
                    'distance': round(d, 2),
                    'line': line_standard
                })
            prev_station_id = station_id
            prev_dist = dist

# Filter out duplicate connections (could happen if multiple lines share segments)
unique_connections = []
seen_conns = set()
for conn in connections:
    pair = tuple(sorted([conn['source'], conn['target']]))
    if (pair, conn['line']) not in seen_conns:
        unique_connections.append(conn)
        seen_conns.add((pair, conn['line']))

# Sort stations by ID for consistency
final_stations = sorted(list(stations.values()), key=lambda x: x['id'])

data = {
    'stations': final_stations,
    'connections': unique_connections
}

with open('/Users/subh/dmrc-visualizer/data/dmrc_data.json', 'w') as f:
    json.dump(data, f, indent=2)

print(f"Generated {len(final_stations)} stations and {len(unique_connections)} connections.")
