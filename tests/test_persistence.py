"""CLI regression using synthetic data only."""
import csv
import pathlib
import subprocess
import sys
import tempfile

binary = str(pathlib.Path(sys.argv[1]).resolve())
with tempfile.TemporaryDirectory() as tmp:
    root = pathlib.Path(tmp)
    data = root / "data"
    data.mkdir()
    (data / "Cars.txt").write_text("License,Type,TotalPaid,IsCharging,IsInQueue,DummyValue\n200,FAST,0,0,0,0\n100,FAST,0,0,0,0\n300,FAST,0,0,0,0\n")
    (data / "Stations.txt").write_text("ID,Name,NumPorts,CoordX,CoordY\n20,Middle,0,0,0\n10,Left,0,1,1\n30,Right,0,3,3\n")
    (data / "Ports.txt").write_text("StationID,PortNum,Type,Status,Year,Month,Day,Hour,Min,CarLicense\n")
    (data / "LineOfCars.txt").write_text("CarLicense,StationID\n")
    def run(commands):
        subprocess.run([binary], input=commands, text=True, cwd=root, check=True, timeout=15, capture_output=True)
    def rows(name, key):
        with (data / name).open() as stream:
            return {r[key]: r for r in csv.DictReader(stream)}
    run("12\n200\n13\n20\n0\n")
    assert set(rows("Cars.txt", "License")) == {"100", "300"}
    stations = rows("Stations.txt", "ID")
    assert set(stations) == {"10", "30"}
    assert stations["30"]["Name"] == "Right"
    assert float(stations["30"]["CoordX"]) == 3
    original = {p.name: p.read_bytes() for p in data.iterdir()}
    # A directory where a staged file must be created forces a write failure.
    (data / "Ports.txt.tmp").mkdir()
    run("0\n")
    assert all((data / name).read_bytes() == content for name, content in original.items())
    assert not (data / "Cars.txt.tmp").exists()

    # A missing input or malformed row must never trigger a partial-state save.
    if (data / "Ports.txt.tmp").is_dir():
        (data / "Ports.txt.tmp").rmdir()
    (data / "LineOfCars.txt").unlink()
    originals = {p.name:p.read_bytes() for p in data.iterdir()}
    result = subprocess.run([binary], input="0\n", text=True, cwd=root, timeout=15, capture_output=True)
    assert result.returncode != 0
    assert {p.name:p.read_bytes() for p in data.iterdir()} == originals
    (data / "LineOfCars.txt").write_text("CarLicense,StationID\ninvalid row\n")
    originals = {p.name:p.read_bytes() for p in data.iterdir()}
    result = subprocess.run([binary], input="0\n", text=True, cwd=root, timeout=15, capture_output=True)
    assert result.returncode != 0
    assert {p.name:p.read_bytes() for p in data.iterdir()} == originals

    # Referentially invalid but syntactically valid input must not be rewritten.
    (data / "LineOfCars.txt").write_text("CarLicense,StationID\nmissing,10\n")
    originals = {p.name:p.read_bytes() for p in data.iterdir()}
    result = subprocess.run([binary], input="0\n", text=True, cwd=root, timeout=15, capture_output=True)
    assert result.returncode != 0
    assert {p.name:p.read_bytes() for p in data.iterdir()} == originals
